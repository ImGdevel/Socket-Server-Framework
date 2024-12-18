#include "EventHandler.h"
#include "Logger.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

unordered_map<string, HandlerFunc> EventHandler::createHandlers() const {
    return {
        {"LOGIN", [this](auto session, auto message) { this->onLogin(session, message); }},
        {"CHAT", [this](auto session, auto message) { this->onChat(session, message); }},
        {"ECHO", [this](auto session, auto message) { this->onEcho(session, message); }},
        {"DELAY", [this](auto session, auto message) { this->onDelay(session, message); }},
        {"TASK", [this](auto session, auto message) { this->onTask(session, message); }}
    };
}

void EventHandler::onLogin(const shared_ptr<ClientSession>& session, const string& message) const {
    if (session) {
        std::this_thread::sleep_for(std::chrono::seconds(10));

        string echoMessage = "LOGIN: " + message;
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void EventHandler::onChat(const shared_ptr<ClientSession>& session, const string& message) const {
    if (session) {
        std::this_thread::sleep_for(std::chrono::microseconds(100));

        string echoMessage = "CHAT: " + message;
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void EventHandler::onEcho(const shared_ptr<ClientSession>& session, const string& message) const {
    if (session) {
        string echoMessage = "ECHO: " + message;
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void EventHandler::onDelay(const shared_ptr<ClientSession>& session, const string& message) const {
    if (session) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        string echoMessage = "DELAY: " + message;
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void EventHandler::onTask(const shared_ptr<ClientSession>& session, const string& message) const {
    if (session) {
        std::this_thread::sleep_for(std::chrono::seconds(3));

        string echoMessage = "TASK: " + message;
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}
