#ifndef CLIENT_SESSION_MANAGER_H
#define CLIENT_SESSION_MANAGER_H

#include "ClientSessionTCP.h"
#include "ClientSessionUDP.h"
#include <unordered_map>
#include <memory>
#include <mutex>

class ClientSessionManager {
private:
    std::unordered_map<int, std::shared_ptr<ClientSession>> sessions;
    std::mutex sessionMutex;

public:
    // 새로운 세션 추가
    void addSession(int sessionId, std::shared_ptr<ClientSession> session) {
        std::lock_guard<std::mutex> lock(sessionMutex);
        sessions[sessionId] = std::move(session);
    }

    // 기존 세션 조회
    std::shared_ptr<ClientSession> getSession(int sessionId) {
        std::lock_guard<std::mutex> lock(sessionMutex);
        auto it = sessions.find(sessionId);
        return (it != sessions.end()) ? it->second : nullptr;
    }

    // 세션 삭제
    void removeSession(int sessionId) {
        std::lock_guard<std::mutex> lock(sessionMutex);
        sessions.erase(sessionId);
    }


    // 모든 세션 종료
    void closeAllSession(){
        for (auto& [socket, session] : sessions) {
            if (socket >= 0) {
                close(socket);
            }
        }
    }

};

#endif
