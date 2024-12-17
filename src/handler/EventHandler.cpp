#include "EventHandler.h"
#include <iostream>

using namespace std;

unordered_map<string, HandlerFunc> EventHandler::createHandlers() const {
    return {
        {"LOGIN", [this](auto session, auto message) { this->onLogin(session, message); }},
        {"CHAT", [this](auto session, auto message) { this->onChat(session, message); }},
        {"ECHO", [this](auto session, auto message) { this->onEcho(session, message); }}
    };
}

void EventHandler::onLogin(const shared_ptr<ClientSession>& session, const string& message) const {
    cout << "Handling LOGIN: " << message << endl;
}

void EventHandler::onChat(const shared_ptr<ClientSession>& session, const string& message) const {
    cout << "Handling CHAT: " << message << endl;
}


void EventHandler::onEcho(const shared_ptr<ClientSession>& session, const string& message) const {
    if (session) {
        string echoMessage = "Echo: " + message;
        cout << "send message : " << echoMessage << endl;
        session->sendMessage(echoMessage);
    }
}