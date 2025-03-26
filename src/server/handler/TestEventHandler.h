#ifndef TESTEVENTHANDLER_H
#define TESTEVENTHANDLER_H

#include "IEventHandler.h"
#include "ClientRequest.h"

class TestEventHandler : public IEventHandler {
public:
    std::unordered_map<std::string, HandlerFunc> createHandlers() const;

private:
    void onLogin(const ClientRequest& ClientRequest) const;
    void onChat(const ClientRequest& ClientRequest) const;
    void onEcho(const ClientRequest& ClientRequest) const;
    void onDelay(const ClientRequest& ClientRequest) const;
    void onTask(const ClientRequest& ClientRequest) const;
    void onJoinRoom(const ClientRequest& ClientRequest) const;
    void onRoomMessage(const ClientRequest& ClientRequest) const;
    void onListRooms(const ClientRequest& ClientRequest) const;
    void onCreateRoom(const ClientRequest& ClientRequest) const;
    void onRemoveRoom(const ClientRequest& ClientRequest) const;
    void onLeaveRoom(const ClientRequest& ClientRequest) const;
};

#endif
