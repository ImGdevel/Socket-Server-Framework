#include "ClientSession.h"
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <stdexcept>
#include <vector>

using namespace std;

ClientSession::ClientSession(int socket) 
    : clientSocket(socket), active(true), processing(false) {
    if (clientSocket < 0) {
        throw runtime_error("Invalid client socket");
    }
}

ClientSession::~ClientSession() {
    closeSession();
}

int ClientSession::getSocket() const {
    return clientSocket;
}

//수신 버퍼에 추가
void ClientSession::appendToBuffer(const char* data, size_t size) {
    receiveBuffer.insert(receiveBuffer.end(), data, data + size);
}

// 활성화 상태 확인
bool ClientSession::isActive() const {
    return active;
}

// 해당 작업의 작업 상태 지정
void ClientSession::setProcessing(bool state) {
    processing = state;
}

// 해당 세션이 작업 상태 확인
bool ClientSession::isProcessing() const {
    return processing;
}

// 메시지 전송
void ClientSession::sendMessage(const string& message) {
    uint32_t messageSize = htonl(static_cast<uint32_t>(message.size()));

    string sendBuffer(reinterpret_cast<char*>(&messageSize), sizeof(messageSize));
    sendBuffer += message;

    if (send(clientSocket, sendBuffer.c_str(), sendBuffer.size(), 0) < 0) {
        throw runtime_error("Failed to send message");
    }
}

// 세션 종료
void ClientSession::closeSession() {
    if (active) {
        close(clientSocket);
        active = false;
    }
}

// 수신 메시지 파싱
bool ClientSession::extractMessage(string& message) {
    if (receiveBuffer.size() < sizeof(uint32_t)) {
        return false;
    }

    uint32_t messageLength = 0;
    memcpy(&messageLength, receiveBuffer.data(), sizeof(uint32_t));
    messageLength = ntohl(messageLength);

    if (receiveBuffer.size() < sizeof(uint32_t) + messageLength) {
        return false;
    }

    message = string(receiveBuffer.begin() + sizeof(uint32_t), receiveBuffer.begin() + sizeof(uint32_t) + messageLength);
    receiveBuffer.erase(receiveBuffer.begin(), receiveBuffer.begin() + sizeof(uint32_t) + messageLength);
    return true;
}

// 현재 방 ID 설정
void ClientSession::setCurrentRoom(const string& roomId) {
    currentRoomId = roomId;
}

// 현재 방 ID 가져오기
string ClientSession::getCurrentRoom() const {
    return currentRoomId;
}
