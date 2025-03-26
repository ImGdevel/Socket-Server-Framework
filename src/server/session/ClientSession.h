#ifndef CLIENT_SESSION_H
#define CLIENT_SESSION_H

#include <string>

class ClientSession {
public:
    virtual ~ClientSession() = default;
    
    virtual bool sendMessage(const std::string& message) = 0;
    virtual void closeSession() = 0;
    virtual bool isActive() const = 0;
};

#endif
