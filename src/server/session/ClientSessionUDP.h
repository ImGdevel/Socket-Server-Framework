#ifndef CLIENT_SESSION_UDP_H
#define CLIENT_SESSION_UDP_H

#include "IClientSession.h"
#include "Logger.h"
#include <string>

class ClientSessionUDP : public IClientSession {
private:
    std::string clientIp;
    uint16_t clientPort;
    bool active;
    std::string sessionId;

public:
    ClientSessionUDP(const std::string& ip, uint16_t port) 
        : clientIp(ip), clientPort(port), active(true) {}

    ClientSessionUDP(sockaddr_in clientAddr) {
        clientIp = inet_ntoa(clientAddr.sin_addr);
        clientPort = ntohs(clientAddr.sin_port);
        active = true;
    }

    void sendMessage(const std::string& message) override {
        // todo : UDP 메시지 전송
        Logger::info(message);
    }

    void closeSession() override {
        active = false;
    }

    bool isActive() const override {
        return active;
    }

    void setSessionId(const std::string& id) {
        sessionId = id;
    }

    std::string getSessionId() const override {
        return sessionId;
    }
};

#endif