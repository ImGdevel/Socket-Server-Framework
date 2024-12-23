#ifndef MESSAGEDISPATCHER_H
#define MESSAGEDISPATCHER_H

#include <unordered_map>
#include <utility>
#include "../handler/HandlerTypes.h"

class MessageDispatcher {
public:
    void registerHandler(const std::string& type, HandlerFunc handler);
    void handleEvent(const std::shared_ptr<ClientSession>& session, const std::string& message);
    std::pair<std::string, std::string> extractMessageTypeAndContent(const std::string& message) const;

private:
    std::unordered_map<std::string, HandlerFunc> handlers;
};

#endif
