#ifndef MESSAGEDISPATCHERFACTORY_H
#define MESSAGEDISPATCHERFACTORY_H

#include "MessageDispatcher.h"
#include "HandlerConfigurator.h"
#include "memory"
#include "../handler/IEventHandler.h"

class MessageDispatcherFactory {
public:
    static std::unique_ptr<MessageDispatcher> createDispatcher(const std::string& format, IEventHandler& handler);

private:
    static std::unique_ptr<MessageDispatcher> createJSONDispatcher(IEventHandler& handler);
    static std::unique_ptr<MessageDispatcher> createJSONRapidDispatcher(IEventHandler& handler);
};

#endif