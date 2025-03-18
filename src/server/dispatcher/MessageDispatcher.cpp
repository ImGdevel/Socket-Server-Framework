#include "MessageDispatcher.h"
#include "EventRegistry.h"
#include "Logger.h"

using namespace std;

MessageDispatcher::MessageDispatcher(unique_ptr<IParser> parser, unique_ptr<EventRegistry> registry) 
    : parser(move(parser)), eventRegistry(move(registry)) {}


void MessageDispatcher::handleEvent(const shared_ptr<ClientSession>& session, const string& message) {
    auto parsedMessage = parser->parse(message);
    if (!parsedMessage) {
        Logger::error("Failed to parse message: " + message);
        return;
    }

    std::string type = parsedMessage->getType();
    ClientRequest request(session, move(parsedMessage));
    
    eventRegistry->dispatchEvent(type, request);
}