#include "ClientSession.h"
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <cstring>

using namespace std;

ClientSession::ClientSession(int socket) : clientSocket(socket), active(true) {
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
    receiveBuffer.append(data, size);
}

bool ClientSession::isActive() const {
    return active;
}

void ClientSession::closeSession() {
    if (active) {
        close(clientSocket);
        active = false;
    }
}

// 수신 버퍼에 데이터 추출
bool ClientSession::extractMessage(string& message) {
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

    // 완성된 메시지 추출 및 버퍼 제거
    message = receiveBuffer.substr(sizeof(uint32_t), messageLength);
    receiveBuffer.erase(0, sizeof(uint32_t) + messageLength);
    return true;
}