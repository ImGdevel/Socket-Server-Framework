#include "HandlerConfigurator.h"
#include "MessageDispatcher.h"
#include "../handler/EventHandler.h"
#include <iostream>

void HandlerConfigurator::registerHandlers(MessageDispatcher& dispatcher, const EventHandler& handler) {
    auto handlers = handler.createHandlers();
    for (const auto& [type, handlerFunc] : handlers) {
        dispatcher.registerHandler(type, handlerFunc);
    }
}
