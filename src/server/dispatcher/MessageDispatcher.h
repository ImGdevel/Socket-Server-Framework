#ifndef MESSAGEDISPATCHER_H
#define MESSAGEDISPATCHER_H

#include "EventRegistry.h"
#include "HandlerTypes.h"
#include "IParser.h"
#include <unordered_map>
#include <utility>
#include <memory>


class MessageDispatcher {
public:
    MessageDispatcher(std::unique_ptr<EventRegistry> registry);

    void handleEvent(const ClientRequest& ClientRequest);

private:
    std::unique_ptr<EventRegistry> eventRegistry;

};

#endif
