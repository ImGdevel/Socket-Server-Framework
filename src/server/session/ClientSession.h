#ifndef CLIENT_SESSION_H
#define CLIENT_SESSION_H

#include <string> 
#include <vector>

class ClientSession {
public:
    explicit ClientSession(int socket);
    ~ClientSession();

    void appendToBuffer(const char* data, size_t size);
    bool extractMessage(std::string& message);
    
    void sendMessage(const std::string& message);

    int getSocket() const;
    bool isActive() const;
    void setProcessing(bool state);
    bool isProcessing() const;
    void closeSession();
    
    void setCurrentRoom(const std::string& roomId);
    std::string getCurrentRoom() const;

private:
    int clientSocket;
    bool active;
    bool processing;

    std::vector<char> receiveBuffer;

    std::string currentRoomId;
};

#endif
