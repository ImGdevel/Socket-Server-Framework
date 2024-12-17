#ifndef MESSAGEDISPATCHER_H
#define MESSAGEDISPATCHER_H

#include "../common/HandlerTypes.h"
#include <unordered_map>
#include <utility>

class MessageDispatcher {
public:
    void registerHandler(const std::string& type, HandlerFunc handler);
    void handleEvent(const std::shared_ptr<ClientSession>& session, const std::string& message);
    std::pair<std::string, std::string> extractMessageTypeAndContent(const std::string& message) const;

private:
    std::unordered_map<std::string, HandlerFunc> handlers;
};

#endif
