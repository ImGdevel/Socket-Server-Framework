#include "MessageDispatcher.h"
#include <iostream>

using namespace std;

void MessageDispatcher::registerHandler(const string& type, HandlerFnc handler) {
    handlers[type] = handler;
}

void MessageDispatcher::handleEvent(const std::shared_ptr<ClientSession>& session, const string& message) {
    auto [type, content] = extractMessageTypeAndContent(message);

    if (type.empty()) {
        cerr << "Invalid message format: " << message << endl;
        return;
    }

    cout << session->getSocket() << " socket event! >> type: " << type << " | message: " << content << endl;

    auto it = handlers.find(type);
    if (it != handlers.end()) {
        it->second(session, content);
    } else {
        cerr << "No handler found for type: " << type << endl;
    }
}

// 메시지 타입과 내용을 추출하는 메서드
pair<string, string> MessageDispatcher::extractMessageTypeAndContent(const string& message) const {
    auto delimiterPos = message.find(":");
    if (delimiterPos == string::npos) {
        return {"", ""};
    }
    string type = message.substr(0, delimiterPos);
    string content = message.substr(delimiterPos + 1);
    return {type, content};
}
