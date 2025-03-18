#pragma once
#include <unordered_map>
#include <functional>
#include "../handler/ClientRequest.h"
#include "../handler/IEventHandler.h"
#include "Logger.h"

class EventRegistry {
public:

    // dispatch가 이벤트 참조
    void dispatchEvent(const std::string& eventType, const ClientRequest& request) {
        auto it = handlers.find(eventType);
        if (it != handlers.end()) {
            it->second(request);
        } else {
            Logger::error("No handler found for event: " + eventType);
        }
    }

    // IEventHandler로 부터 이벤트를 등록
    void registerEvents(IEventHandler& handler) {
        auto handlersMap = handler.createHandlers();
        for (const auto& [type, handlerFunc] : handlersMap) {
            Logger::info("In " + type);
            handlers[type] = handlerFunc;
        }
    }

    // 별도의 이벤트를 직접 등록
    void registerEvent(const std::string& eventType, HandlerFunc handler) {
        handlers[eventType] = std::move(handler);
    }

private:
    std::unordered_map<std::string, HandlerFunc> handlers;
};
