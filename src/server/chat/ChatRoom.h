#ifndef CHATROOM_H
#define CHATROOM_H

#include <string>
#include <unordered_set>
#include <memory>
#include <mutex>

class ClientSession;

class ChatRoom {
public:
    ChatRoom(const std::string& roomId, const std::string& roomName);

    void addClient(const std::shared_ptr<ClientSession>& client);
    void removeClient(const std::shared_ptr<ClientSession>& client);
    void broadcastMessage(const std::string& message, const std::shared_ptr<ClientSession>& sender);

    std::string getId() const;
    std::string getName() const;
    void setName(const std::string& newName);

private:
    std::string id;
    std::string name;
    std::unordered_set<std::shared_ptr<ClientSession>> clients;
    std::mutex roomMutex;
};

#endif
