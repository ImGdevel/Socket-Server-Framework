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
        Builder() : port(0), workerCount(0), eventHandler(nullptr), eventRegistry(std::make_unique<EventRegistry>()) {}

        Builder& setPort(int p);
        Builder& setWorkerCount(int count);
        Builder& setMessageType(const std::string& type);
        Builder& setEventHandler(IEventHandler& handler);
        std::unique_ptr<Server> build();

    private:
        int port;
        int workerCount;
        std::string messageDispatcherType;
        std::unique_ptr<EventRegistry> eventRegistry;

        void validate() const;
    };

    ~Server();

    void run();
    void terminate();

private:
    friend class Builder;

    Server(int port, int workerCount, std::unique_ptr<ThreadPool> tp, std::unique_ptr<MessageDispatcher> md);

    const int port;
    const int workerCount;

    std::unique_ptr<Reactor> reactor; 
    std::unique_ptr<ThreadPool> threadPool;
    std::unique_ptr<MessageDispatcher> messageDispatcher;

    void initialize();
};

#endif 