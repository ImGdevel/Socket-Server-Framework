#ifndef CLIENT_SESSION_MANAGER_TCP_H
#define CLIENT_SESSION_MANAGER_TCP_H

#include "ClientSessionTCP.h"
#include <unordered_map>
#include <memory>
#include <mutex>

class ClientSessionManagerTCP {
private:
    std::unordered_map<int, std::shared_ptr<ClientSessionTCP>> sessions;
    std::mutex sessionMutex;

public:
    void addSession(int sessionId, std::shared_ptr<ClientSessionTCP> session) {
        std::lock_guard<std::mutex> lock(sessionMutex);
        sessions[sessionId] = std::move(session);
    }

    std::shared_ptr<ClientSessionTCP> getSession(int sessionId) {
        std::lock_guard<std::mutex> lock(sessionMutex);
        auto it = sessions.find(sessionId);
        return (it != sessions.end()) ? it->second : nullptr;
    }

    void removeSession(int sessionId) {
        std::lock_guard<std::mutex> lock(sessionMutex);
        sessions.erase(sessionId);
    }

    void closeAllSessions() {
        std::lock_guard<std::mutex> lock(sessionMutex);
        for (auto& [id, session] : sessions) {
            session->closeSession();
        }
        sessions.clear();
    }
};

#endif
