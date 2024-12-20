#include "HandlerConfigurator.h"
#include "MessageDispatcher.h"
#include "../handler/IEventHandler.h"
#include <iostream>

void HandlerConfigurator::registerHandlers(MessageDispatcher& dispatcher, const IEventHandler& handler) {
    auto handlers = handler.createHandlers();
    for (const auto& [type, handlerFunc] : handlers) {
        dispatcher.registerHandler(type, handlerFunc);
    }
}
