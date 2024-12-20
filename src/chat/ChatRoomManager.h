#ifndef CHATROOMMANAGER_H
#define CHATROOMMANAGER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>
#include "ChatRoom.h"

class ChatRoomManager {
public:
    static ChatRoomManager& getInstance();

    std::shared_ptr<ChatRoom> getOrCreateRoom(const std::string& roomName);
    std::shared_ptr<ChatRoom> getRoom(const std::string& roomName);

private:
    ChatRoomManager() = default;
    ~ChatRoomManager() = default;

    ChatRoomManager(const ChatRoomManager&) = delete;
    ChatRoomManager& operator=(const ChatRoomManager&) = delete;

    std::unordered_map<std::string, std::shared_ptr<ChatRoom>> chatRooms;
    std::mutex managerMutex;
};

#endif
