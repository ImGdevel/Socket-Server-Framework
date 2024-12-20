#include "ChatRoom.h"
#include "Logger.h"
#include <algorithm>
#include <iostream>
#include "../session/ClientSession.h"

using namespace std;

ChatRoom::ChatRoom(const std::string& roomName) : name(roomName) {

}

// 클라이언트 추가
void ChatRoom::addClient(const shared_ptr<ClientSession>& client) {
    lock_guard<mutex> lock(roomMutex);
    clients.insert(client);

    string joinMessage = client->getSocket() + " has joined the room: " + name;
    Logger::debug(joinMessage);
    broadcastMessage(joinMessage, nullptr);
}

// 클라이언트 제거
void ChatRoom::removeClient(const shared_ptr<ClientSession>& client) {
    lock_guard<mutex> lock(roomMutex);
    clients.erase(client);

    string leaveMessage = client->getSocket() + " has left the room: " + name;
    Logger::debug(leaveMessage);
    broadcastMessage(leaveMessage, nullptr);
}

// 메시지 브로드캐스트
void ChatRoom::broadcastMessage(const string& message, const shared_ptr<ClientSession>& sender) {
    lock_guard<mutex> lock(roomMutex);

    for (const auto& client : clients) {
        if (client != sender) {
            client->sendMessage(message);
        }
    }
}

// 채팅방 이름 조회
string ChatRoom::getName() const {
    return name;
}
