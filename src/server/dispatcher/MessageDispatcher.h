#ifndef MESSAGEDISPATCHER_H
#define MESSAGEDISPATCHER_H

#include "../handler/HandlerTypes.h"
#include "../messages/parser/IParser.h"
#include <unordered_map>
#include <utility>
#include <memory>

class MessageDispatcher {
public:
    MessageDispatcher(std::unique_ptr<IParser> parser);

    void registerHandler(const std::string& type, HandlerFunc handler);
    void handleEvent(const std::shared_ptr<ClientSession>& session, const std::string& message);

private:
    std::unordered_map<std::string, HandlerFunc> handlers;
    std::unique_ptr<IParser> parser;
};

#endif
