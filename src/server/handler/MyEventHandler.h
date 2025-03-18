#pragma once
#include "IEventHandler.h"
#include "Logger.h"
#include "../chat/ChatRoomManager.h"
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <unordered_map>

class MyEventHandler : public IEventHandler {
public:
    std::unordered_map<std::string, HandlerFunc> createHandlers() const override {
        return {
            {"LOGIN", [this](const ClientRequest& request) { onLogin(request); }},
            {"ECHO", [this](const ClientRequest& request) { onEcho(request); }},
            {"DELAY", [this](const ClientRequest& request) { onDelay(request); }},
            {"CHAT", [this](const ClientRequest& request) { onChat(request); }},
            {"TASK", [this](const ClientRequest& request) { onTask(request); }},
        };
    }

private:
    void onLogin(const ClientRequest& request) const;
    void onChat(const ClientRequest& request) const;
    void onEcho(const ClientRequest& request) const;
    void onDelay(const ClientRequest& request) const;
    void onTask(const ClientRequest& request) const;
    std::string createJsonResponse(const std::string& type, const std::string& content) const;
};
