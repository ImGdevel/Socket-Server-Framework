#ifndef CLIENT_SESSION_TCP_H
#define CLIENT_SESSION_TCP_H

#include "ClientSession.h"
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <stdexcept>

class ClientSessionTCP : public ClientSession {
private:
public:
    explicit ClientSessionTCP(int socket);
    ~ClientSessionTCP();

    void closeSession() override;
    bool sendMessage(const std::string& message) override;
    bool isActive() const override;
    
    int getSocket() const;
    void setProcessing(bool state);
    bool isProcessing() const;

    void appendToBuffer(const char* data, size_t size);
    bool extractMessage(std::string& message);

private:
    int clientSocket;
    bool active;
    bool processing;
    std::vector<char> receiveBuffer;
};

#endif
