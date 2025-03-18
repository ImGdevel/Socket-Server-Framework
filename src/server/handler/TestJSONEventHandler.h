#ifndef TESTJSONEVENTHANDLER_H
#define TESTJSONEVENTHANDLER_H

#include "IEventHandler.h"
#include "JsonMessage.h"
#include "ClientRequest.h"
#include <unordered_map>

class TestJSONEventHandler : public IEventHandler {
public:
    std::unordered_map<std::string, HandlerFunc> createHandlers() const;

private:
    void onLogin(const ClientRequest& ClientRequest) const;
    void onChat(const ClientRequest& ClientRequest) const;
    void onEcho(const ClientRequest& ClientRequest) const;
    void onDelay(const ClientRequest& ClientRequest) const;
    void onTask(const ClientRequest& ClientRequest) const;

    std::string createJsonResponse(const std::string& type, const std::string& content) const;
};

#endif