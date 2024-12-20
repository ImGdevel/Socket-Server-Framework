#include "ChatRoom.h"
#include "Logger.h"
#include <algorithm>
#include <iostream>
#include "../session/ClientSession.h"

using namespace std;

ChatRoom::ChatRoom(const string& roomId, const string& roomName)
    : id(roomId), name(roomName) {}

void ChatRoom::addClient(const shared_ptr<ClientSession>& client) {
    lock_guard<mutex> lock(roomMutex);
    clients.insert(client);
}

void ChatRoom::removeClient(const shared_ptr<ClientSession>& client) {
    lock_guard<mutex> lock(roomMutex);
    clients.erase(client);
}

void ChatRoom::broadcastMessage(const string& message, const shared_ptr<ClientSession>& sender) {
    lock_guard<mutex> lock(roomMutex);
    for (const auto& client : clients) {
        if (client != sender) {
            client->sendMessage(message);
        }
    }
}

string ChatRoom::getId() const {
    return id;
}

string ChatRoom::getName() const {
    return name;
}

void ChatRoom::setName(const string& newName) {
    lock_guard<mutex> lock(roomMutex);
    name = newName;
}