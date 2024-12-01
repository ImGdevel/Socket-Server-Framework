#ifndef REACTOR_H
#define REACTOR_H

#include <vector>
#include <functional>
#include <unordered_map>
#include <memory>

class ClientSession;
class ThreadPool;

class Reactor {
public:
    explicit Reactor(int port, ThreadPool& threadPool); 
    ~Reactor();

    void start();
    void stop();

private:
    int port;
    int serverSocket;
    int epollFd;

    static const int MAX_EVENTS = 1024;

    bool running = false;
    
    void setupServerSocket();
    void setupIOMultiplexing();
    void acceptConnection();
    void handleClientEvent(int clientSocket);
    void setNonBlocking(int socket);

    std::unordered_map<int, std::shared_ptr<ClientSession>> clientSessions;
    ThreadPool& threadPool;
};

#endif
