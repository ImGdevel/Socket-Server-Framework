#ifndef CLIENT_SESSION_UDP_H
#define CLIENT_SESSION_UDP_H

#include "ClientSession.h"
#include "Logger.h"
#include <arpa/inet.h>
#include <string>
#include <sys/socket.h>

class ClientSessionUDP : public ClientSession {
private:
    std::string clientIp;
    uint16_t clientPort;
    int serverSocket;
    bool active;

public:
    ClientSessionUDP(int sock, const std::string& ip, uint16_t port);
    ClientSessionUDP(int sock, sockaddr_in clientAddr);

    bool sendMessage(const std::string& message) override;
    void closeSession() override;
    bool isActive() const override;
    std::string getClientAddress() const;
};

#endif
