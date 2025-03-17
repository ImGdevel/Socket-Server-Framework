#ifndef SERVER_H
#define SERVER_H

#include <memory>
#include "reactor/Reactor.h"
#include "threadpool/ThreadPool.h"

class Server {
public:
    class Builder {
    public:
        Builder& setPort(int p);
        Builder& setWorkerCount(int count);
        std::unique_ptr<Server> build();
    private:
        int port;
        int workerCount;
    };

    ~Server();

    void run();
    void terminate();

private:
    friend class Builder;

    Server(int port, int workerCount);

    int port;
    int workerCount;

    std::unique_ptr<Reactor> reactor; 
    std::unique_ptr<ThreadPool> threadPool;
    std::unique_ptr<MessageDispatcher> messageDispatcher;

    void initialize();
};

#endif 