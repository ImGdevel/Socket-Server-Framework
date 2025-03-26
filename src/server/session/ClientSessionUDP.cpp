#include "ClientSessionUDP.h"
#include "Logger.h"

ClientSessionUDP::ClientSessionUDP(int sock, const std::string& ip, uint16_t port)
    : serverSocket(sock), clientIp(ip), clientPort(port), active(true) {}

ClientSessionUDP::ClientSessionUDP(int sock, sockaddr_in clientAddr)
    : serverSocket(sock), active(true) {
    clientIp = inet_ntoa(clientAddr.sin_addr);
    clientPort = ntohs(clientAddr.sin_port);
}

// 메시지 전송
bool ClientSessionUDP::sendMessage(const std::string& message) {
    if (!active) {
        Logger::warning("Attempt to send message to inactive UDP session: " + getClientAddress());
        return false;
    }

    sockaddr_in clientAddr{};
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(clientPort);
    inet_pton(AF_INET, clientIp.c_str(), &clientAddr.sin_addr);

    ssize_t sentBytes = sendto(serverSocket, message.c_str(), message.size(), 0,
                               (struct sockaddr*)&clientAddr, sizeof(clientAddr));
    if (sentBytes < 0) {
        Logger::error("Failed to send UDP message");
        return false;
    }
    return true;
}

// 세션 종료
void ClientSessionUDP::closeSession() {
    active = false;
    Logger::info("Closed UDP session: " + getClientAddress());
}

// 활성화 여부 확인
bool ClientSessionUDP::isActive() const {
    return active;
}

// 클라이언트 주소 반환
std::string ClientSessionUDP::getClientAddress() const {
    return clientIp + ":" + std::to_string(clientPort);
}
