#ifndef TESTEVENTHANDLER_H
#define TESTEVENTHANDLER_H

#include "IEventHandler.h"

class TestEventHandler : public IEventHandler {
public:
    std::unordered_map<std::string, HandlerFunc> createHandlers() const;

private:
    void onLogin(const std::shared_ptr<ClientSession>& session, const MessagePtr& message) const;
    void onChat(const std::shared_ptr<ClientSession>& session,  const MessagePtr& message) const;
    void onEcho(const std::shared_ptr<ClientSession>& session,  const MessagePtr& message) const;
    void onDelay(const std::shared_ptr<ClientSession>& session,  const MessagePtr& message) const;
    void onTask(const std::shared_ptr<ClientSession>& session,  const MessagePtr& message) const;
    void onJoinRoom(const std::shared_ptr<ClientSession>& session,  const MessagePtr& message) const;
    void onRoomMessage(const std::shared_ptr<ClientSession>& session,  const MessagePtr& message) const;
    void onListRooms(const std::shared_ptr<ClientSession>& session,  const MessagePtr& message) const;
    void onCreateRoom(const std::shared_ptr<ClientSession>& session,  const MessagePtr& message) const;
    void onRemoveRoom(const std::shared_ptr<ClientSession>& session,  const MessagePtr& message) const;
    void onLeaveRoom(const std::shared_ptr<ClientSession>& session,  const MessagePtr& message) const;
};

#endif
