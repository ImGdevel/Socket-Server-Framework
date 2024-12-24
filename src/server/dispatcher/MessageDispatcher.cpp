#include "MessageDispatcher.h"
#include "Logger.h"

using namespace std;

MessageDispatcher::MessageDispatcher(unique_ptr<IParser> parser) : parser(move(parser)) {
}

void MessageDispatcher::registerHandler(const string& type, HandlerFunc handler) {
    handlers[type] = handler;
}

void MessageDispatcher::handleEvent(const std::shared_ptr<ClientSession>& session, const string& message) {
    auto [type, content] = parser->parse(message);

    if (type.empty()) {
        Logger::error("Invalid message format: " + message);
        return;
    }

    Logger::info("Socket " + to_string(session->getSocket()) + " event! >> type: " + type + " | message: " + content);

    auto it = handlers.find(type);
    if (it != handlers.end()) {
        it->second(session, content);
    } else {
        Logger::error("No handler found for type: " + type);
    }
}

// 메시지 타입과 내용을 추출
pair<string, string> MessageDispatcher::extractMessageTypeAndContent(const string& message) const {
    auto delimiterPos = message.find(":");
    if (delimiterPos == string::npos) {
        return {"", ""};
    }
    string type = message.substr(0, delimiterPos);
    string content = message.substr(delimiterPos + 1);
    return {type, content};
}
