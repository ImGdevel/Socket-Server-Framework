#ifndef SERVER_H
#define SERVER_H

#include <memory>
#include "reactor/Reactor.h"
#include "threadpool/ThreadPool.h"

class Server {
public:
    static Server& getInstance(int port, int workerCount);

    ~Server();

    void run();
    void terminate();

private:
    Server(int port, int workerCount);
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    int port;
    int workerCount;

    std::unique_ptr<Reactor> reactor; 
    std::unique_ptr<ThreadPool> threadPool;
    std::unique_ptr<MessageDispatcher> messageDispatcher;

    void initialize();
};

#endif 