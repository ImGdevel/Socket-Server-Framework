#include "MessageDispatcher.h"
#include "Logger.h"
#include <memory>

using namespace std;

MessageDispatcher::MessageDispatcher() {}

void MessageDispatcher::registerHandler(const string& type, HandlerFunc handler) {
    handlers[type] = handler;
}

void MessageDispatcher::handleEvent(const ClientRequest& ClientRequest) {
    const auto& session = ClientRequest.getSession();
    const auto& message = ClientRequest.getMessage();

    string type = message->getType();
    auto it = handlers.find(type);
    
    if (it != handlers.end()) {
        it->second(ClientRequest);
    } else {
        Logger::error("No handler found for type: " + type);
    }
}
