#ifndef CLIENTSESSION_H
#define CLIENTSESSION_H

#include "IClientSession.h"
#include "ClientSessionTCP.h"
#include "ClientSessionUDP.h"
#include <memory>
#include <netinet/in.h>

class ClientSession {
private:
    std::unique_ptr<IClientSession> session;

    explicit ClientSession(std::unique_ptr<IClientSession> sess) 
        : session(std::move(sess)) {}

public:
    // TCP 세션 생성
    static std::shared_ptr<ClientSession> createTCP(int socket) {
        return std::make_shared<ClientSession>(ClientSession(std::make_unique<ClientSessionTCP>(socket)));
    }

    // UDP 세션 생성
    static std::shared_ptr<ClientSession> createUDP(sockaddr_in clientAddr) {
        return std::make_shared<ClientSession>(ClientSession(std::make_unique<ClientSessionUDP>(clientAddr)));
    }

    void sendMessage(const std::string& message) {
        session->sendMessage(message);
    }

    void closeSession() {
        session->closeSession();
    }

    bool isActive() const {
        return session->isActive();
    }
};

#endif
