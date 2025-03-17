#include "MessageDispatcher.h"
#include "Logger.h"
#include "../handler/EventRegistry.h"
#include <memory>

using namespace std;

MessageDispatcher::MessageDispatcher(unique_ptr<IParser> parser) 
    : parser(move(parser)) {}

void MessageDispatcher::registerHandler(const string& type, HandlerFunc handler) {
    handlers[type] = handler;
}

void MessageDispatcher::handleEvent(const shared_ptr<ClientSession>& session, const string& message) {
    auto parsedMessage = parser->parse(message);
    if (!parsedMessage) {
        Logger::error("Failed to parse message: " + message);
        return;
    }

    string type = parsedMessage->getType();
    const auto& handlers = EventRegistry::getHandlers();
    auto it = handlers.find(type);
    
    if (it != handlers.end()) {
        ClientRequest request(session, std::move(parsedMessage));
        it->second(request);
    } else {
        Logger::error("No handler found for type: " + type);
    }
}
