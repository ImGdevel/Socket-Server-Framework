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
    void closeSession();
    

private:
    // 클라이언트 정보
    int clientSocket;
    bool active;
    
    // 수신 버퍼
    std::vector<char> receiveBuffer;
};

#endif
