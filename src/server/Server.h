#ifndef SERVER_H
#define SERVER_H

#include <memory>
#include <string>
#include "reactor/Reactor.h"
#include "threadpool/ThreadPool.h"
#include "handler/IEventHandler.h"
#include "dispatcher/EventRegistry.h"

class Server {
public:
    class Builder {
    public:
        Builder& setPort(int p);
        Builder& setWorkerCount(int count);
        Builder& setMessageType(const std::string& type);
        Builder& setEventHandler(IEventHandler& handler);
        std::unique_ptr<Server> build();
    private:
        int port;
        int workerCount;
        std::string messageDispatcherType;
        IEventHandler* eventHandler;
    };

    ~Server();

    void run();
    void terminate();

private:
    friend class Builder;

    Server(int port, int workerCount, std::unique_ptr<ThreadPool> tp, std::unique_ptr<MessageDispatcher> md);

    int port;
    int workerCount;

    std::unique_ptr<Reactor> reactor; 
    std::unique_ptr<ThreadPool> threadPool;
    std::unique_ptr<MessageDispatcher> messageDispatcher;
    std::unique_ptr<EventRegistry> eventRegistry;

    void initialize();
};

#endif 