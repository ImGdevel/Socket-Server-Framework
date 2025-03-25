#ifndef SERVER_H
#define SERVER_H

#include "Reactor.h"
#include "ThreadPool.h"
#include "IEventHandler.h"
#include "EventRegistry.h"
#include "IFilter.h"
#include "FilterChain.h"
#include "ParserFactory.h"
#include "MessageProcessor.h"
#include "TransportProtocol.h"
#include <memory>
#include <string>

class Server {
public:
    class Builder {
    public:
        Builder() : port(0), workerCount(0), protocol(TransportProtocol::TCP), eventRegistry(std::make_unique<EventRegistry>()), filterChain(std::make_unique<FilterChain>()) {}

        Builder& setPort(int p);
        Builder& setWorkerCount(int count);
        Builder& setTransportProtocol(const std::string& pt);
        Builder& setEventHandler(IEventHandler& handler);
        Builder& setMessageType(const std::string& type);
        Builder& addFilter(std::unique_ptr<IFilter> filter);
        std::unique_ptr<Server> build();
        

    private:
        int port;
        int workerCount;
        TransportProtocol protocol;
        std::string messageDispatcherType;
        std::unique_ptr<EventRegistry> eventRegistry;
        std::unique_ptr<FilterChain> filterChain;

        void validate() const;
        void throwError(const std::string& msg) const;
    };

    ~Server();

    void run();
    void terminate();

private:
    friend class Builder;

    Server(int port, int workerCount, TransportProtocol protocol, std::unique_ptr<ThreadPool> tp, std::unique_ptr<MessageProcessor> mp);

    const int port;
    const int workerCount;
    const TransportProtocol protocol;

    std::unique_ptr<Reactor> reactor; 
    std::unique_ptr<ThreadPool> threadPool;
    std::unique_ptr<MessageProcessor> messageProcessor;

    void initialize();
};

#endif 