#pragma once

#include "HandlerTypes.h"
#include "ClientRequest.h"
#include "IEventHandler.h"
#include "Logger.h"
#include <unordered_map>
#include <functional>

class EventRegistry {
public:

    void dispatchEvent(const std::string& eventType, const ClientRequest& request) {
        auto it = handlers.find(eventType);
        if (it != handlers.end()) {
            it->second(request);
        } else {
            Logger::error("No handler found for event: " + eventType);
        }
    }

    void registerHandlers(const IEventHandler& handler) {
        auto handlers = handler.createHandlers();
        for (const auto& [type, handlerFunc] : handlers) {
            registerEvent(type, handlerFunc);
            Logger::info("eroll" + type);
        }
    }


    void registerEvent(const std::string& type, HandlerFunc handler) {
        handlers[type] = handler;
    }

private:
    std::unordered_map<std::string, HandlerFunc> handlers;
};
