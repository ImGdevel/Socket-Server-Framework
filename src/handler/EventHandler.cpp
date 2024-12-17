#include "EventHandler.h"
#include <iostream>

using namespace std;

unordered_map<string, HandlerFunc> EventHandler::createHandlers() const {
    return {
        {"LOGIN", [this](auto session, auto message) { this->onLogin(session, message); }},
        {"CHAT", [this](auto session, auto message) { this->onChat(session, message); }}
    };
}

void EventHandler::onLogin(const shared_ptr<ClientSession>& session, const string& message) const {
    cout << "Handling LOGIN: " << message << endl;
}

void EventHandler::onChat(const shared_ptr<ClientSession>& session, const string& message) const {
    cout << "Handling CHAT: " << message << endl;
}
