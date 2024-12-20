#include "ChatRoomManager.h"
#include "UUIDGenerator.h"

using namespace std;

ChatRoomManager& ChatRoomManager::getInstance() {
    static ChatRoomManager instance;
    return instance;
}

shared_ptr<ChatRoom> ChatRoomManager::getOrCreateRoom(const string& roomName) {
    lock_guard<mutex> lock(managerMutex);

    for (const auto& [id, room] : chatRooms) {
        if (room->getName() == roomName) {
            return room;
        }
    }

    string newRoomId = UUIDGenerator::generateUUID();
    auto newRoom = make_shared<ChatRoom>(newRoomId, roomName);
    chatRooms[newRoomId] = newRoom;

    return newRoom;
}

shared_ptr<ChatRoom> ChatRoomManager::getRoom(const string& roomId) {
    lock_guard<std::mutex> lock(managerMutex);

    auto it = chatRooms.find(roomId);
    if (it != chatRooms.end()) {
        return it->second;
    }
    return nullptr;
}