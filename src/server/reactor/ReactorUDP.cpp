#include "ReactorUDP.h"
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <iostream>

ReactorUDP::ReactorUDP(int port, ThreadPool& threadPool, MessageProcessor& messageProcessor)
    : Reactor(port, threadPool, messageProcessor), running(false) {}

ReactorUDP::~ReactorUDP() {
    stop();
}

void ReactorUDP::start() {
    setupServerSocket();
    running = true;
    receiveLoop();
}

void ReactorUDP::stop() {
    running = false;
    if (serverSocket >= 0) {
        close(serverSocket);
    }
}

void ReactorUDP::setupServerSocket() {
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0) {
        throw std::runtime_error("Failed to create UDP socket");
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        close(serverSocket);
        throw std::runtime_error("Failed to bind UDP socket");
    }
}

void ReactorUDP::receiveLoop() {
    char buffer[BUFFER_SIZE];
    sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    while (running) {
        ssize_t received = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, 
                                    (struct sockaddr*)&clientAddr, &addrLen);
        if (received > 0) {
            buffer[received] = '\0';
            handleIncomingMessage(clientAddr, buffer);
        }
    }
}

void ReactorUDP::handleIncomingMessage(const sockaddr_in& clientAddr, const std::string& message) {
    int sessionId = clientAddr.sin_addr.s_addr ^ clientAddr.sin_port;
    auto session = sessionManager.getSession(sessionId);

    if (!session) {
        session = std::make_shared<ClientSessionUDP>(serverSocket, clientAddr);
        sessionManager.addSession(sessionId, session);
    }

    messageProcessor.processMessage(session, message);
}
