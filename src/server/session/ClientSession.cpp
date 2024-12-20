#include "ClientSession.h"
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <cstring>

using namespace std;

ClientSession::ClientSession(int socket) : clientSocket(socket), active(true), processing(false) {
    if(clientSocket < 0){
        throw runtime_error("Invalid clinet socket");
    }
}

ClientSession::~ClientSession(){
    closeSession();
}

int ClientSession::getSocket() const {
    return clientSocket;
}

void ClientSession::appendToBuffer(const char* data, size_t size) {
    receiveBuffer.insert(receiveBuffer.end(), data, data + size);
}

bool ClientSession::isActive() const {
    return active;
}

void ClientSession::setProcessing(bool state){
    processing = state;
}

bool ClientSession::isProcessing() const{
    return processing;
}

void ClientSession::sendMessage(const std::string& message) {
    uint32_t messageSize = htonl(static_cast<uint32_t>(message.size()));

    std::string sendBuffer(reinterpret_cast<char*>(&messageSize), sizeof(messageSize));
    sendBuffer += message;

    if (send(clientSocket, sendBuffer.c_str(), sendBuffer.size(), 0) < 0) {
        throw std::runtime_error("Failed to send message");
    }
}

void ClientSession::closeSession() {
    if (active) {
        close(clientSocket);
        active = false;
    }
}

bool ClientSession::extractMessage(std::string& message) {
    // 메시지 헤더를 읽을 수 있을 만큼 데이터가 없음
    if (receiveBuffer.size() < sizeof(uint32_t)) {
        return false;
    }

    // 메시지 길이 추출
    uint32_t messageLength = 0;
    std::memcpy(&messageLength, receiveBuffer.data(), sizeof(uint32_t));
    messageLength = ntohl(messageLength);

    // 메시지 전체가 도착했는지 확인
    if (receiveBuffer.size() < sizeof(uint32_t) + messageLength) {
        return false;
    }

    // 완성된 메시지 추출 및 버퍼에서 제거
    message = std::string(receiveBuffer.begin() + sizeof(uint32_t), receiveBuffer.begin() + sizeof(uint32_t) + messageLength);
    receiveBuffer.erase(receiveBuffer.begin(), receiveBuffer.begin() + sizeof(uint32_t) + messageLength);
    return true;
}
