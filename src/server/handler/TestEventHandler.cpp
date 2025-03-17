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
        {"LOGIN", [this](const ClientRequest& ClientRequest) { this->onLogin(ClientRequest); }},
        {"ECHO", [this](const ClientRequest& ClientRequest) { this->onEcho(ClientRequest); }},
        {"DELAY", [this](const ClientRequest& ClientRequest) { this->onDelay(ClientRequest); }},
        {"JOIN", [this](const ClientRequest& ClientRequest) { this->onJoinRoom(ClientRequest); }},
        {"CHAT", [this](const ClientRequest& ClientRequest) { this->onRoomMessage(ClientRequest); }},
        {"ROOM_LIST", [this](const ClientRequest& ClientRequest) { this->onListRooms(ClientRequest); }},
        {"ROOM_REMOVE", [this](const ClientRequest& ClientRequest) { this->onRemoveRoom(ClientRequest); }},
        {"LEAVE", [this](const ClientRequest& ClientRequest) { this->onLeaveRoom(ClientRequest); }},
    };
}

void TestEventHandler::onLogin(const ClientRequest& ClientRequest) const {
    const auto& session = ClientRequest.getSession();
    const auto& message = ClientRequest.getMessage();
    if (session) {
        string echoMessage = "MSG: " + message->serialize();
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void TestEventHandler::onChat(const ClientRequest& ClientRequest) const {
    const auto& session = ClientRequest.getSession();
    const auto& message = ClientRequest.getMessage();
    if (session) {
        string echoMessage = "MSG: " + message->serialize();
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void TestEventHandler::onEcho(const ClientRequest& ClientRequest) const {
    const auto& session = ClientRequest.getSession();
    const auto& message = ClientRequest.getMessage();
    if (session) {
        string echoMessage = "MSG: " + message->serialize();
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void TestEventHandler::onDelay(const ClientRequest& ClientRequest) const {
    const auto& session = ClientRequest.getSession();
    const auto& message = ClientRequest.getMessage();
    if (session) {
        string echoMessage = "MSG: " + message->serialize();
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void TestEventHandler::onTask(const ClientRequest& ClientRequest) const {
    const auto& session = ClientRequest.getSession();
    const auto& message = ClientRequest.getMessage();
    if (session) {
        string echoMessage = "MSG: " + message->serialize();
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void TestEventHandler::onJoinRoom(const ClientRequest& ClientRequest) const {
    const auto& session = ClientRequest.getSession();
    const auto& message = ClientRequest.getMessage();
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

void TestEventHandler::onRoomMessage(const ClientRequest& ClientRequest) const {
    const auto& session = ClientRequest.getSession();
    const auto& message = ClientRequest.getMessage();
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

void TestEventHandler::onListRooms(const ClientRequest& ClientRequest) const {
    const auto& session = ClientRequest.getSession();
    if (session) {
        auto roomIds = ChatRoomManager::getInstance().getRooms();
        string response = "ROOMS: ";
        for (const auto& id : roomIds) {
            response += id + " ";
        }
        session->sendMessage(response.empty() ? "MSG: No rooms available." : response);
    }
}

void TestEventHandler::onCreateRoom(const ClientRequest& ClientRequest) const {
    const auto& session = ClientRequest.getSession();
    const auto& message = ClientRequest.getMessage();
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

void TestEventHandler::onRemoveRoom(const ClientRequest& ClientRequest) const {
    const auto& session = ClientRequest.getSession();
    const auto& message = ClientRequest.getMessage();
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

void TestEventHandler::onLeaveRoom(const ClientRequest& ClientRequest) const {
    const auto& session = ClientRequest.getSession();
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
