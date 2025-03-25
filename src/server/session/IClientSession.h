#ifndef I_CLIENT_SESSION_H
#define I_CLIENT_SESSION_H

#include <string>

class IClientSession {
public:
    virtual ~IClientSession() = default;
    
    virtual void sendMessage(const std::string& message) = 0;
    virtual void closeSession() = 0;
    virtual bool isActive() const = 0;
    virtual std::string getSessionId() const = 0;
};

#endif
