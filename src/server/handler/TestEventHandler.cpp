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
        string echoMessage = "LOGIN: " + message;
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void TestEventHandler::onChat(const shared_ptr<ClientSession>& session, const string& message) const {
    if (session) {
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
        string echoMessage = "DELAY: " + message;
        Logger::info("send message : " + echoMessage);
        this_thread::sleep_for(chrono::seconds(1));
        session->sendMessage(echoMessage);
    }
}

void TestEventHandler::onTask(const shared_ptr<ClientSession>& session, const string& message) const {
    if (session) {
        string echoMessage = "TASK: " + message;
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void TestEventHandler::onJoinRoom(const shared_ptr<ClientSession>& session, const string& message) const {
    if (session) {
        istringstream iss(message);
        string roomName;
        iss >> roomName;

        auto chatRoom = ChatRoomManager::getInstance().getOrCreateRoom(roomName);
        chatRoom->addClient(session);

        session->setCurrentRoom(chatRoom->getId());
        session->sendMessage("Joined room: " + chatRoom->getName());
    }
}

void TestEventHandler::onRoomMessage(const shared_ptr<ClientSession>& session, const string& message) const {
    if (session) {
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
}

void TestEventHandler::onListRooms(const std::shared_ptr<ClientSession>& session) const {
    if (session) {
        auto roomIds = ChatRoomManager::getInstance().getRooms();
        std::string response = "Available rooms: ";
        for (const auto& id : roomIds) {
            response += id + " ";
        }
        session->sendMessage(response.empty() ? "No rooms available." : response);
    }
}

void TestEventHandler::onRemoveRoom(const std::shared_ptr<ClientSession>& session, const std::string& message) const {
    if (session) {
        std::istringstream iss(message);
        std::string roomId;
        iss >> roomId;

        if (ChatRoomManager::getInstance().removeRoom(roomId)) {
            session->sendMessage("Room " + roomId + " removed successfully.");
        } else {
            session->sendMessage("Room " + roomId + " not found or cannot be removed.");
        }
    }
}


void TestEventHandler::onLeaveRoom(const std::shared_ptr<ClientSession>& session) const {
    if (session) {
        auto currentRoomId = session->getCurrentRoom();
        if (currentRoomId.empty()) {
            session->sendMessage("You are not in any room.");
            return;
        }

        auto chatRoom = ChatRoomManager::getInstance().getRoom(currentRoomId);
        if (chatRoom) {
            chatRoom->removeClient(session);
            session->setCurrentRoom("");
            session->sendMessage("You have left the room: " + chatRoom->getName());
        } else {
            session->sendMessage("The room no longer exists.");
        }
    }
}
