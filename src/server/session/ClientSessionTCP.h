#ifndef CLIENT_SESSION_TCP_H
#define CLIENT_SESSION_TCP_H

#include "IClientSession.h"
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <stdexcept>

class ClientSessionTCP : public IClientSession {
private:
    int clientSocket;
    bool active;
    std::string sessionId;
    std::vector<char> receiveBuffer;

public:
    explicit ClientSessionTCP(int socket) 
        : clientSocket(socket), active(true) {}

    void sendMessage(const std::string& message) override {
        uint32_t messageSize = htonl(static_cast<uint32_t>(message.size()));
        std::string sendBuffer(reinterpret_cast<char*>(&messageSize), sizeof(messageSize));
        sendBuffer += message;

        if (send(clientSocket, sendBuffer.c_str(), sendBuffer.size(), 0) < 0) {
            throw std::runtime_error("Failed to send message");
        }
    }

    bool extractMessage(std::string& message) {
        if (receiveBuffer.size() < sizeof(uint32_t)) {
            return false;
        }

        uint32_t messageLength = 0;
        memcpy(&messageLength, receiveBuffer.data(), sizeof(uint32_t));
        messageLength = ntohl(messageLength);

        if (receiveBuffer.size() < sizeof(uint32_t) + messageLength) {
            return false;
        }

        message = std::string(receiveBuffer.begin() + sizeof(uint32_t),
                              receiveBuffer.begin() + sizeof(uint32_t) + messageLength);
        receiveBuffer.erase(receiveBuffer.begin(), receiveBuffer.begin() + sizeof(uint32_t) + messageLength);
        return true;
    }

    void appendToBuffer(const char* data, size_t size) {
        receiveBuffer.insert(receiveBuffer.end(), data, data + size);
    }

    void closeSession() override {
        if (active) {
            close(clientSocket);
            active = false;
        }
    }

    bool isActive() const override {
        return active;
    }

    void updateSocket(int newSocket) {
        if (clientSocket > 0) {
            close(clientSocket);
        }
        clientSocket = newSocket;
    }

    void setSessionId(const std::string& id) {
        sessionId = id;
    }

    std::string getSessionId() const override {
        return sessionId;
    }
};

#endif
