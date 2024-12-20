#include "ChatRoomManager.h"

using namespace std;

ChatRoomManager& ChatRoomManager::getInstance() {
    static ChatRoomManager instance;
    return instance;
}

shared_ptr<ChatRoom> ChatRoomManager::getOrCreateRoom(const string& roomName) {
    lock_guard<mutex> lock(managerMutex);
    auto it = chatRooms.find(roomName);
    if (it == chatRooms.end()) {
        auto newRoom = make_shared<ChatRoom>(roomName);
        chatRooms[roomName] = newRoom;
        return newRoom;
    }
    return it->second;
}

shared_ptr<ChatRoom> ChatRoomManager::getRoom(const string& roomName) {
    lock_guard<mutex> lock(managerMutex);
    auto it = chatRooms.find(roomName);
    return (it != chatRooms.end()) ? it->second : nullptr;
}
