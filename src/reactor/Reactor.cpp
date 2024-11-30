#include "Reactor.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>

using namespace std;

Reactor::Reactor(int port) : port(port), serverSocket(-1), running(false) {
    setupServerSocket();
}

Reactor::~Reactor() {
    if (serverSocket >= 0) {
        close(serverSocket);
    }
}

void Reactor::setupServerSocket(){
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw runtime_error("Failed to create socket");
    }

    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
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
    running = true;
}

void Reactor::start() {
    cout << "Reactor started, waiting for connections..." << endl;
    while (running) {
        acceptConnection();
    }
}

void Reactor::stop(){
    running = false;
}

void Reactor::acceptConnection() {
    sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);

    if (clientSocket >= 0) {
        cout << "New client connected!" << endl;
        // todo : 클라이언트 처리 핸들러 등록
    }
}
