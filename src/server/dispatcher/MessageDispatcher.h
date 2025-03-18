#ifndef MESSAGEDISPATCHER_H
#define MESSAGEDISPATCHER_H

#include "../handler/HandlerTypes.h"
#include "../messages/parser/IParser.h"
#include <unordered_map>
#include <utility>
#include <memory>
#include "EventRegistry.h"

class MessageDispatcher {
public:
    MessageDispatcher(std::unique_ptr<EventRegistry> registry);

    void handleEvent(const ClientRequest& ClientRequest);

private:
    std::unique_ptr<EventRegistry> eventRegistry;

};

#endif
