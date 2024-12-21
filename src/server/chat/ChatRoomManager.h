#ifndef CHATROOMMANAGER_H
#define CHATROOMMANAGER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <vector>
#include "ChatRoom.h"

class ChatRoomManager {
public:
    static ChatRoomManager& getInstance();

     ~ChatRoomManager() = default;

    std::shared_ptr<ChatRoom> getOrCreateRoom(const std::string& roomName);
    std::shared_ptr<ChatRoom> getRoom(const std::string& roomId);
    std::vector<std::string> getRooms();
    bool removeRoom(const std::string& roomId);


private:
    ChatRoomManager() = default;
    ChatRoomManager(const ChatRoomManager&) = delete;
    ChatRoomManager& operator=(const ChatRoomManager&) = delete;

    std::unordered_map<std::string, std::shared_ptr<ChatRoom>> chatRooms;
    std::mutex managerMutex;
};

#endif
