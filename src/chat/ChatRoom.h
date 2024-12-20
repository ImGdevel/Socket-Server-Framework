#ifndef CHATROOM_H
#define CHATROOM_H

#include <string>
#include <unordered_set>
#include <memory>
#include <mutex>

class ClientSession;

class ChatRoom {
public:
    explicit ChatRoom(const std::string& roomName);

    void addClient(const std::shared_ptr<ClientSession>& client);
    void removeClient(const std::shared_ptr<ClientSession>& client);
    void broadcastMessage(const std::string& message, const std::shared_ptr<ClientSession>& sender);

    std::string getName() const;

private:
    std::string name;
    std::unordered_set<std::shared_ptr<ClientSession>> clients;
    std::mutex roomMutex;
};

#endif
