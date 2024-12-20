#include "TestEventHandler.h"
#include "Logger.h"
#include "../chat/ChatRoomManager.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>

using namespace std;

unordered_map<string, HandlerFunc> TestEventHandler::createHandlers() const {
    return {
        {"LOGIN", [this](auto session, auto message) { this->onLogin(session, message); }},
        {"CHAT", [this](auto session, auto message) { this->onChat(session, message); }},
        {"ECHO", [this](auto session, auto message) { this->onEcho(session, message); }},
        {"DELAY", [this](auto session, auto message) { this->onDelay(session, message); }},
        {"TASK", [this](auto session, auto message) { this->onTask(session, message); }}
    };
}

void TestEventHandler::onLogin(const shared_ptr<ClientSession>& session, const string& message) const {
    if (session) {
        std::this_thread::sleep_for(std::chrono::seconds(10));

        string echoMessage = "LOGIN: " + message;
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void TestEventHandler::onChat(const shared_ptr<ClientSession>& session, const string& message) const {
    if (session) {
        std::this_thread::sleep_for(std::chrono::microseconds(100));

        string echoMessage = "CHAT: " + message;
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void TestEventHandler::onEcho(const shared_ptr<ClientSession>& session, const string& message) const {
    if (session) {
        string echoMessage = "ECHO: " + message;
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void TestEventHandler::onDelay(const shared_ptr<ClientSession>& session, const string& message) const {
    if (session) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        string echoMessage = "DELAY: " + message;
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void TestEventHandler::onTask(const shared_ptr<ClientSession>& session, const string& message) const {
    if (session) {
        std::this_thread::sleep_for(std::chrono::seconds(3));

        string echoMessage = "TASK: " + message;
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void TestEventHandler::onJoinRoom(const shared_ptr<ClientSession>& session, const string& message) const {
    istringstream iss(message);
    string roomName;
    iss >> roomName;

    auto chatRoom = ChatRoomManager::getInstance().getOrCreateRoom(roomName);
    chatRoom->addClient(session);

    session->setCurrentRoom(chatRoom->getId());
    session->sendMessage("Joined room: " + chatRoom->getName());
}

void TestEventHandler::onRoomMessage(const shared_ptr<ClientSession>& session, const string& message) const {
    auto roomId = session->getCurrentRoom();
    if (roomId.empty()) {
        session->sendMessage("You are not in a chat room.");
        return;
    }

    auto chatRoom = ChatRoomManager::getInstance().getRoom(roomId);
    if (chatRoom) {
        chatRoom->broadcastMessage(message, session);
    } else {
        session->sendMessage("The chat room no longer exists.");
    }
}
