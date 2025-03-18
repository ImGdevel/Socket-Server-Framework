#ifndef MESSAGEDISPATCHER_H
#define MESSAGEDISPATCHER_H

#include "../handler/HandlerTypes.h"
#include "../messages/parser/IParser.h"
#include <unordered_map>
#include <utility>
#include <memory>

class MessageDispatcher {
public:
    MessageDispatcher();

    void registerHandler(const std::string& type, HandlerFunc handler);
    void handleEvent(const ClientRequest& ClientRequest);

private:
    std::unordered_map<std::string, HandlerFunc> handlers;
};

#endif
