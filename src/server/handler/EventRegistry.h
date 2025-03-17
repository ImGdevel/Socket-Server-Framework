#ifndef EVENT_REGISTRY_H
#define EVENT_REGISTRY_H

#include "HandlerTypes.h"
#include "ClientRequest.h"
#include <unordered_map>
#include <string>

class EventRegistry {
public:
    class Builder {
    public:
        Builder& on(const std::string& eventType, HandlerFunc handler) {
            EventRegistry::registerHandler(eventType, std::move(handler));
            return *this;
        }
    };

    static bool registerHandler(const std::string& eventType, HandlerFunc handler) {
        getInstance().handlers[eventType] = std::move(handler);
        return true;
    }

    static Builder registerHandlers() {
        return Builder();
    }

    static const std::unordered_map<std::string, HandlerFunc>& getHandlers() {
        return getInstance().handlers;
    }

private:
    static EventRegistry& getInstance() {
        static EventRegistry instance;
        return instance;
    }

    std::unordered_map<std::string, HandlerFunc> handlers;
};

#endif 