#ifndef MESSAGEDISPATCHER_H
#define MESSAGEDISPATCHER_H

#include "../handler/HandlerTypes.h"
#include "parser/IParser.h"
#include <unordered_map>
#include <utility>
#include <memory>

class MessageDispatcher {
public:
    MessageDispatcher(unique_ptr<IParser> parser);

    void registerHandler(const std::string& type, HandlerFunc handler);
    void handleEvent(const std::shared_ptr<ClientSession>& session, const std::string& message);
    std::pair<std::string, std::string> extractMessageTypeAndContent(const std::string& message) const;

private:
    std::unordered_map<std::string, HandlerFunc> handlers;
    unique_ptr<IParser> parser;
};

#endif
