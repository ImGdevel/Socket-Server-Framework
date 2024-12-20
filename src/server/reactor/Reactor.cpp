#include "Reactor.h"
#include "Logger.h"
#include "../session/ClientSession.h"
#include "../threadpool/ThreadPool.h"
#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <stdexcept>
#include <memory>

using namespace std;

Reactor::Reactor(int port, ThreadPool& threadPool, MessageDispatcher messageDispatcher)
        : port(port), serverSocket(-1), epollFd(-1), running(false), threadPool(threadPool), messageDispatcher(messageDispatcher) {
    setupServerSocket();
    setupIOMultiplexing();
}

Reactor::~Reactor() {
    stop();
    for (auto& [socket, session] : clientSessions) {
        safeClose(socket);
    }
    clientSessions.clear();
    safeClose(epollFd);
    safeClose(serverSocket);
}

void Reactor::setupServerSocket() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw runtime_error("Failed to create socket");
    }

    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        safeClose(serverSocket);
        throw runtime_error("Failed to set socket options");
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        safeClose(serverSocket);
        throw runtime_error("Failed to bind socket");
    }

    if (listen(serverSocket, LISTEN_BACKLOG) < 0) {
        safeClose(serverSocket);
        throw runtime_error("Failed to listen on socket");
    }

    setNonBlocking(serverSocket);
}

void Reactor::setupIOMultiplexing() {
    epollFd = epoll_create1(0);
    if (epollFd < 0) {
        throw runtime_error("Failed to create epoll instance");
    }

    epoll_event event{};
    event.events = EPOLLIN;
    event.data.fd = serverSocket;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &event) < 0) {
        safeClose(epollFd);
        throw runtime_error("Failed to add server socket to epoll");
    }
}


void Reactor::start() {
    Logger::info("Reactor started, waiting for connections...");
    running = true;

    epoll_event events[MAX_EVENTS];

    while (running) {
        int eventCount = epoll_wait(epollFd, events, MAX_EVENTS, -1);
        if (eventCount < 0) {
            if (errno == EINTR) {
                continue;
            }
            throw runtime_error("epoll_wait failed");
        }

        for (int i = 0; i < eventCount; ++i) {
            if (events[i].data.fd == serverSocket) {
                acceptConnection();
            } else {
                handleClientEvent(events[i].data.fd);
            }
        }
    }
}

void Reactor::stop() {
    running = false;
}

void Reactor::acceptConnection() {
    while (true) {
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);

        if (clientSocket < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            }
            Logger::error("accept failed: " + string(strerror(errno)));
            return;
        }

        Logger::info("New client connected: " + to_string(clientSocket));
        setNonBlocking(clientSocket);

        addClientSession(clientSocket);

        epoll_event event{};
        event.events = EPOLLIN | EPOLLET;
        event.data.fd = clientSocket;
        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &event) < 0) {
            Logger::error("Failed to add client socket to epoll: " + to_string(clientSocket));
            removeClientSession(clientSocket);
        }
    }
}

void Reactor::handleClientEvent(int clientSocket) {
    char buffer[BUFFER_SIZE];

    auto sessionIter = clientSessions.find(clientSocket);
    if (sessionIter == clientSessions.end()) {
        Logger::error("Invalid client socket: " + to_string(clientSocket));
        removeClientSession(clientSocket);
        return;
    }

    auto& session = sessionIter->second;
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesRead < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return;
        }
        Logger::error("recv failed for socket: " + std::to_string(clientSocket));
        removeClientSession(clientSocket);
        return;
    } else if (bytesRead == 0) {
        Logger::info("Client disconnected: " + std::to_string(clientSocket));
        removeClientSession(clientSocket);
        return;
    } else {
        session->appendToBuffer(buffer, bytesRead);
        
        if(session->isProcessing()){
            return;
        }

        string message;
        if (session->extractMessage(message)) {
            session->setProcessing(true);
            threadPool.enqueueTask([this, session, message]() {
                messageDispatcher.handleEvent(session, message);
                session->setProcessing(false);
            });
        }
    }
}

void Reactor::addClientSession(int clientSocket) {
    clientSessions[clientSocket] = std::make_shared<ClientSession>(clientSocket);
}

void Reactor::removeClientSession(int clientSocket) {
    if (epoll_ctl(epollFd, EPOLL_CTL_DEL, clientSocket, nullptr) < 0) {
        Logger::warning("Failed to remove client socket from epoll: " + to_string(clientSocket));
    }

    auto it = clientSessions.find(clientSocket);
    if (it != clientSessions.end()) {
        it->second->closeSession();
        clientSessions.erase(it);
    }

    safeClose(clientSocket);
}

void Reactor::setNonBlocking(int socket) {
    int flags = fcntl(socket, F_GETFL, 0);
    if (flags < 0) {
        throw runtime_error("Failed to get socket flags");
    }

    if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) < 0) {
        throw runtime_error("Failed to set socket to non-blocking");
    }
}

void Reactor::safeClose(int socket) {
    if (socket >= 0) {
        close(socket);
    }
}
