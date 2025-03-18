#ifndef MESSAGEDISPATCHER_H
#define MESSAGEDISPATCHER_H

#include "../handler/HandlerTypes.h"
#include "../messages/parser/IParser.h"
#include "EventRegistry.h"
#include <unordered_map>
#include <utility>
#include <memory>

class MessageDispatcher {
public:
    MessageDispatcher(std::unique_ptr<IParser> parser, std::unique_ptr<EventRegistry> registry);

    void handleEvent(const std::shared_ptr<ClientSession>& session, const std::string& message);

private:
    std::unique_ptr<EventRegistry> eventRegistry;
    std::unique_ptr<IParser> parser;
};

#endif
