#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "IEventHandler.h"

class EventHandler : public IEventHandler {
public:
    std::unordered_map<std::string, HandlerFunc> createHandlers() const;

private:
    void onLogin(const std::shared_ptr<ClientSession>& session, const std::string& message) const;
    void onChat(const std::shared_ptr<ClientSession>& session, const std::string& message) const;
    void onEcho(const std::shared_ptr<ClientSession>& session, const std::string& message) const;
    void onDelay(const std::shared_ptr<ClientSession>& session, const std::string& message) const;
    void onTask(const std::shared_ptr<ClientSession>& session, const std::string& message) const;
};

#endif
