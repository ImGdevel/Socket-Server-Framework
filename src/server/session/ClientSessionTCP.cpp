#include "ClientSessionTCP.h"
#include "Logger.h"
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <stdexcept>
#include <vector>

using namespace std;

ClientSessionTCP::ClientSessionTCP(int socket) 
    : clientSocket(socket), active(true), processing(false) {
    if (clientSocket < 0) {
        throw runtime_error("Invalid client socket");
    }
}

ClientSessionTCP::~ClientSessionTCP() {
    closeSession();
}

int ClientSessionTCP::getSocket() const {
    return clientSocket;
}

//수신 버퍼에 추가
void ClientSessionTCP::appendToBuffer(const char* data, size_t size) {
    receiveBuffer.insert(receiveBuffer.end(), data, data + size);
}

// 활성화 상태 확인
bool ClientSessionTCP::isActive() const {
    return active;
}

// 해당 작업의 작업 상태 지정
void ClientSessionTCP::setProcessing(bool state) {
    processing = state;
}

// 해당 세션이 작업 상태 확인
bool ClientSessionTCP::isProcessing() const {
    return processing;
}

// 메시지 전송
bool ClientSessionTCP::sendMessage(const string& message) {
    uint32_t messageSize = htonl(static_cast<uint32_t>(message.size()));

    string sendBuffer(reinterpret_cast<char*>(&messageSize), sizeof(messageSize));
    sendBuffer += message;

    ssize_t sentBytes = send(clientSocket, sendBuffer.c_str(), sendBuffer.size(), 0);
    if (sentBytes < 0) {
        Logger::error("Failed to send message");
        return false;
    }
    return true;
}


// 세션 종료
void ClientSessionTCP::closeSession() {
    if (active) {
        close(clientSocket);
        clientSocket = -1;
        active = false;
    }
}

// 수신 메시지 파싱
bool ClientSessionTCP::extractMessage(string& message) {
    if (receiveBuffer.size() < sizeof(uint32_t)) {
        return false;
    }

    uint32_t messageLength = 0;
    memcpy(&messageLength, receiveBuffer.data(), sizeof(uint32_t));
    messageLength = ntohl(messageLength);

    if (messageLength > 4096) {
        Logger::error("Invalid message size:" + to_string(messageLength));
        receiveBuffer.clear();
        return false;
    }

    if (receiveBuffer.size() < sizeof(uint32_t) + messageLength) {
        return false;
    }

    message = string(receiveBuffer.begin() + sizeof(uint32_t), receiveBuffer.begin() + sizeof(uint32_t) + messageLength);
    receiveBuffer.erase(receiveBuffer.begin(), receiveBuffer.begin() + sizeof(uint32_t) + messageLength);
    return true;
}