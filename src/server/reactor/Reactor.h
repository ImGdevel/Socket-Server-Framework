#ifndef REACTOR_H
#define REACTOR_H

#include "MessageProcessor.h"
#include "ThreadPool.h"
#include <vector>
#include <functional>
#include <unordered_map>
#include <memory>

class ClientSession;
class ThreadPool;
class MessageDispatcher;

class Reactor {
public:
    explicit Reactor(int port, ThreadPool& threadPool, MessageProcessor& messageProcessor);  
    ~Reactor();

    void start();
    void stop();

private:
    const int MAX_EVENTS = 1024;
    const int BUFFER_SIZE = 1024;
    const int LISTEN_BACKLOG = 10;

    int port;
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
    ThreadPool& threadPool;
    MessageProcessor& messageProcessor;
};

#endif
