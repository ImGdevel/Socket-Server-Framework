#include "ReactorTCP.h"
#include "ClientSession.h"
#include "ThreadPool.h"
#include "Logger.h"
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

ReactorTCP::ReactorTCP(int port, ThreadPool& threadPool, MessageProcessor& messageProcessor)
    : Reactor(port, threadPool, messageProcessor), serverSocket(-1), epollFd(-1), running(false) {
    setupServerSocket();
    setupIOMultiplexing();
}

ReactorTCP::~ReactorTCP() {
    stop();
    safeClose(epollFd);
    safeClose(serverSocket);
    Logger::debug("ReactorTCP stopped.");
}

// Server Socket 설정
void ReactorTCP::setupServerSocket() {
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

// I/O Multiplexing 설정
void ReactorTCP::setupIOMultiplexing() {
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

// ReactorTCP 실행
void ReactorTCP::start() {
    Logger::info("ReactorTCP started, waiting for connections...");
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

void ReactorTCP::stop() {
    running = false;
}

// 클라이언트 연결 및 세션 생성
void ReactorTCP::acceptConnection() {
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

        sessionManager.addSession(clientSocket, ClientSession::createTCP(clientSocket));

        epoll_event event{};
        event.events = EPOLLIN | EPOLLET;
        event.data.fd = clientSocket;
        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &event) < 0) {
            Logger::error("Failed to add client socket to epoll: " + to_string(clientSocket));
            sessionManager.removeSession(clientSocket);
        }
    }
}

// 클라이언트 요청 처리
void ReactorTCP::handleClientEvent(int clientSocket) {
    char buffer[BUFFER_SIZE];

    auto session = sessionManager.getSession(clientSocket);
    if (!session) {
        Logger::error("Invalid client socket: " + to_string(clientSocket));
        return;
    }

    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesRead < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return;
        }
        Logger::error("recv failed for socket: " + to_string(clientSocket));
        sessionManager.removeSession(clientSocket);
        return;
    } else if (bytesRead == 0) {
        Logger::info("Client disconnected: " + to_string(clientSocket));
        sessionManager.removeSession(clientSocket);
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
                messageProcessor.processMessage(session, message);
                session->setProcessing(false);
            });
        }
    }
}

// 해당 소캣을 Non-Blocking으로 전환
void ReactorTCP::setNonBlocking(int socket) {
    int flags = fcntl(socket, F_GETFL, 0);
    if (flags < 0) {
        throw runtime_error("Failed to get socket flags");
    }

    if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) < 0) {
        throw runtime_error("Failed to set socket to non-blocking");
    }
}

// socket 안전하게 죵료 
void ReactorTCP::safeClose(int socket) { 
    if (socket >= 0) {
        close(socket);
    }
}