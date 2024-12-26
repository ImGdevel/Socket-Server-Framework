#ifndef TESTJSONEVENTHANDLER_H
#define TESTJSONEVENTHANDLER_H

#include "IEventHandler.h"
#include "JsonMessage.h"
#include <unordered_map>

class TestJSONEventHandler : public IEventHandler {
public:
    std::unordered_map<std::string, HandlerFunc> createHandlers() const;

private:
    void onLogin(const std::shared_ptr<ClientSession>& session, const MessagePtr& message) const;
    void onChat(const std::shared_ptr<ClientSession>& session, const MessagePtr& message) const;
    void onEcho(const std::shared_ptr<ClientSession>& session, const MessagePtr& message) const;
    void onDelay(const std::shared_ptr<ClientSession>& session, const MessagePtr& message) const;
    void onTask(const std::shared_ptr<ClientSession>& session, const MessagePtr& message) const;

    std::string createJsonResponse(const std::string& type, const std::string& content) const;
};

#endif