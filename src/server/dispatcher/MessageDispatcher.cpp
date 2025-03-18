#include "MessageDispatcher.h"
#include "Logger.h"

using namespace std;

MessageDispatcher::MessageDispatcher(unique_ptr<EventRegistry> registry)
    : eventRegistry(move(registry))
{}


void MessageDispatcher::handleEvent(const ClientRequest& ClientRequest) {
    const auto& message = ClientRequest.getMessage();

    string type = message->getType();
    eventRegistry->dispatchEvent(type, ClientRequest);
}