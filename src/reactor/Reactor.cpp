#include "Reactor.h"
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

Reactor::Reactor(int port) : port(port), serverSocket(-1), running(false) {
    setupServerSocket();
    setupIOMultiplexing();

    threadPool = new ThreadPool(10);
}

Reactor::~Reactor() {
    for (auto& [socket, session] : clientSessions) {
        close(socket);
    }
    clientSessions.clear();
    
    if(epollFd >= 0){
        close(epollFd);
    }
    if (serverSocket >= 0) {
        close(serverSocket);
    }
}

// Server Socket 설정
void Reactor::setupServerSocket(){
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw runtime_error("Failed to create socket");
    }

    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        throw runtime_error("Failed to set socket options");
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        throw runtime_error("Failed to bind socket");
    }

    if (listen(serverSocket, 10) < 0) {
        throw runtime_error("Failed to listen on socket");
    }

    setNonBlocking(serverSocket);
}

// I/O Multipexing epoll 설정
void Reactor::setupIOMultiplexing(){
    epollFd = epoll_create1(0);
    if (epollFd < 0) {
        throw runtime_error("Failed to create epoll instance");
    }

    // 서버 소켓을 epoll에 등록
    epoll_event event{};
    event.events = EPOLLIN;
    event.data.fd = serverSocket;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &event) < 0) {
        throw runtime_error("Failed to add server socket to epoll");
    }
}


// 서버 가동
void Reactor::start() {
    cout << "Reactor started, waiting for connections..." << endl;
    running = true;

    epoll_event events[MAX_EVENTS];

    while (running) {
        int eventCount = epoll_wait(epollFd, events, MAX_EVENTS, -1);
        if (eventCount < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("epoll_wait failed");
            break;
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

// 서버 종료
void Reactor::stop(){
    running = false;
}

// 클라이언트 연결
void Reactor::acceptConnection() {
    while (true) {
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);

        if (clientSocket < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            }
            perror("accept failed");
            return;
        }

        cout << "New client connected: " << clientSocket << endl;
        setNonBlocking(clientSocket);

        clientSessions[clientSocket] = make_shared<ClientSession>(clientSocket);

        // 클라이언트 소켓을 epoll에 등록
        epoll_event event{};
        event.events = EPOLLIN | EPOLLET;
        event.data.fd = clientSocket;
        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &event) < 0) {
            perror("Failed to add client socket to epoll");
            clientSessions.erase(clientSocket);
        }
    }
}

// 클라이언트 메시지 수신
void Reactor::handleClientEvent(int clientSocket) {
    char buffer[1024];
    while (true) {
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        cout << "recv clientSocket: " << clientSocket << ", read = " << bytesRead << endl;

        if (bytesRead < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            }
            perror("recv failed");
            clientSessions.erase(clientSocket);
            break;
        } else if (bytesRead == 0) {
            cout << "Client disconnected: " << clientSocket << endl;
            clientSessions.erase(clientSocket);
            break;
        } else {
            auto session = clientSessions[clientSocket];
            session->appendToBuffer(buffer, bytesRead);

            std::string message;
            while (session->extractMessage(message)) {
                threadPool->enqueueTask([this, message, clientSocket]() {
                    std::string response = "Echo: " + message;
                    send(clientSocket, response.data(), response.size(), 0);
                });
            }    
        }
    }
}

// Non-Blocking 설정
void Reactor::setNonBlocking(int socket) {
    int flags = fcntl(socket, F_GETFL, 0);
    if (flags < 0) {
        throw runtime_error("Failed to get socket flags");
    }

    if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) < 0) {
        throw runtime_error("Failed to set socket to non-blocking");
    }
}
