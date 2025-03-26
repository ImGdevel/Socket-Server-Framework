#ifndef REACTOR_TCP_H
#define REACTOR_TCP_H

#include "Reactor.h"
#include "ClientSessionManagerTCP.h"
#include <vector>
#include <unordered_map>

class ReactorTCP : public Reactor {
public:
    explicit ReactorTCP(int port, ThreadPool& threadPool, MessageProcessor& messageProcessor);
    ~ReactorTCP();

    void start() override;
    void stop() override;

private:
    const int MAX_EVENTS = 1024;
    const int BUFFER_SIZE = 1024;
    const int LISTEN_BACKLOG = 10;

    int serverSocket;
    int epollFd;
    bool running = false;

    void setupServerSocket();
    void setupIOMultiplexing();
    void acceptConnection();
    void handleClientEvent(int clientSocket);
    void setNonBlocking(int socket);
    void safeClose(int socket);
    void addClientSession(int socket);
    void removeClientSession(int socket);

    std::unordered_map<int, std::shared_ptr<ClientSession>> clientSessions;
    ClientSessionManagerTCP sessionManager;
};

#endif
