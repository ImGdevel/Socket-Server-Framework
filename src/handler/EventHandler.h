#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

class ClientSession;

using HandlerFunc = std::function<void(const std::shared_ptr<ClientSession>&, const std::string&)>;

class EventHandler {
public:
    std::unordered_map<std::string, HandlerFunc> createHandlers() const;

private:
    void onLogin(const std::shared_ptr<ClientSession>& session, const std::string& message) const;
    void onChat(const std::shared_ptr<ClientSession>& session, const std::string& message) const;
};

#endif
