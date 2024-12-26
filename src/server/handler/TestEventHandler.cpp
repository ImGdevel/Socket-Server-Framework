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
        {"LOGIN", [this](const shared_ptr<ClientSession>& session, const MessagePtr& message) { this->onLogin(session, message); }},
        {"ECHO", [this](const shared_ptr<ClientSession>& session, const MessagePtr& message) { this->onEcho(session, message); }},
        {"DELAY", [this](const shared_ptr<ClientSession>& session, const MessagePtr& message) { this->onDelay(session, message); }},
        {"JOIN", [this](const shared_ptr<ClientSession>& session, const MessagePtr& message) { this->onJoinRoom(session, message); }},
        {"CHAT", [this](const shared_ptr<ClientSession>& session, const MessagePtr& message) { this->onRoomMessage(session, message); }},
        {"ROOM_LIST", [this](const shared_ptr<ClientSession>& session, const MessagePtr& message) { this->onListRooms(session, message); }},
        {"ROOM_REMOVE", [this](const shared_ptr<ClientSession>& session, const MessagePtr& message) { this->onRemoveRoom(session, message); }},
        {"LEAVE", [this](const shared_ptr<ClientSession>& session, const MessagePtr& message) { this->onLeaveRoom(session, message); }},
    };
}

void TestEventHandler::onLogin(const shared_ptr<ClientSession>& session, const MessagePtr& message) const {
    if (session) {
        string echoMessage = "MSG: " + message->serialize();
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void TestEventHandler::onChat(const shared_ptr<ClientSession>& session,  const MessagePtr& message) const {
    if (session) {
        string echoMessage = "MSG: " + message->serialize();
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void TestEventHandler::onEcho(const shared_ptr<ClientSession>& session,  const MessagePtr& message) const {
    if (session) {
        string echoMessage = "MSG: " + message->serialize();
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void TestEventHandler::onDelay(const shared_ptr<ClientSession>& session,  const MessagePtr& message) const {
    if (session) {
        string echoMessage = "MSG: " + message->serialize();
        Logger::info("send message : " + echoMessage);
        //this_thread::sleep_for(chrono::seconds(1));
        session->sendMessage(echoMessage);
    }
}

void TestEventHandler::onTask(const shared_ptr<ClientSession>& session,  const MessagePtr& message) const {
    if (session) {
        string echoMessage = "MSG: " + message->serialize();
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void TestEventHandler::onJoinRoom(const shared_ptr<ClientSession>& session,  const MessagePtr& message) const {
    if (session) {
        istringstream iss(message->serialize());
        string roomName;
        iss >> roomName;

        auto chatRoom = ChatRoomManager::getInstance().getOrCreateRoom(roomName);
        chatRoom->addClient(session);

        session->setCurrentRoom(chatRoom->getId());
        session->sendMessage("MSG: " + chatRoom->getName());
    }
}

void TestEventHandler::onRoomMessage(const shared_ptr<ClientSession>& session,  const MessagePtr& message) const {
    if (session) {
        auto roomId = session->getCurrentRoom();
        if (roomId.empty()) {
            session->sendMessage("MSG: You are not in a chat room.");
            return;
        }

        auto chatRoom = ChatRoomManager::getInstance().getRoom(roomId);
        if (chatRoom) {
            chatRoom->broadcastMessage(message->serialize(), session);
        } else {
            session->sendMessage("MSG: The chat room no longer exists.");
        }
    }
}

void TestEventHandler::onListRooms(const shared_ptr<ClientSession>& session,  const MessagePtr& message) const {
    if (session) {
        auto roomIds = ChatRoomManager::getInstance().getRooms();
        string response = "ROOMS: ";
        for (const auto& id : roomIds) {
            response += id + " ";
        }
        session->sendMessage(response.empty() ? "MSG: No rooms available." : response);
    }
}

void TestEventHandler::onCreateRoom(const shared_ptr<ClientSession>& session,  const MessagePtr& message) const {
    if (session) {
        istringstream iss(message->serialize());
        string roomName;
        iss >> roomName;

        if (ChatRoomManager::getInstance().getOrCreateRoom(roomName)) {
            session->sendMessage("MSG: Room " + roomName + " created successfully.");
        } else {
            session->sendMessage("MSG: Room " + roomName + " not found or cannot be create.");
        }
    }
}

void TestEventHandler::onRemoveRoom(const shared_ptr<ClientSession>& session,  const MessagePtr& message) const {
    if (session) {
        istringstream iss(message->serialize());
        string roomId;
        iss >> roomId;

        if (ChatRoomManager::getInstance().removeRoom(roomId)) {
            session->sendMessage("MSG: Room " + roomId + " removed successfully.");
        } else {
            session->sendMessage("MSG: Room " + roomId + " not found or cannot be removed.");
        }
    }
}


void TestEventHandler::onLeaveRoom(const std::shared_ptr<ClientSession>& session,  const MessagePtr& message) const {
    if (session) {
        auto currentRoomId = session->getCurrentRoom();
        if (currentRoomId.empty()) {
            session->sendMessage("MSG: You are not in any room.");
            return;
        }

        auto chatRoom = ChatRoomManager::getInstance().getRoom(currentRoomId);
        if (chatRoom) {
            chatRoom->removeClient(session);
            session->setCurrentRoom("");
            session->sendMessage("MSG: You have left the room: " + chatRoom->getName());
        } else {
            session->sendMessage("MSG: The room no longer exists.");
        }
    }
}
