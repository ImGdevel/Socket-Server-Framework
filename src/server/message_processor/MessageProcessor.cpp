#include "MessageProcessor.h"

using namespace std;

MessageProcessor::MessageProcessor(unique_ptr<MessageDispatcher> dispatcher, unique_ptr<FilterChain> filterChain, unique_ptr<IParser> parser)
    : dispatcher(move(dispatcher)), filterChain(move(filterChain)), parser(move(parser)) {}

void MessageProcessor::processMessage(shared_ptr<ClientSession> session, const string& message) {
    auto parsedMessage = parser->parse(message);
    if (!parsedMessage) {
        Logger::error("Failed to parse message: " + message);
        return;
    }

    ClientRequest request(session, move(parsedMessage));
    filterChain->doFilter(request);
    dispatcher->handleEvent(request);
}
