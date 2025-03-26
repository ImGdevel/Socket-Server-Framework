#ifndef REACTOR_H
#define REACTOR_H

#include "MessageProcessor.h"
#include "ThreadPool.h"
#include <unordered_map>
#include <memory>

enum class ReactorType { TCP, UDP };

class ClientSession;
class ThreadPool;
class MessageProcessor;

class Reactor {
public:
    explicit Reactor(int port, ThreadPool& threadPool, MessageProcessor& messageProcessor)
        : port(port), threadPool(threadPool), messageProcessor(messageProcessor) {}

    virtual ~Reactor() = default;

    virtual void start() = 0;
    virtual void stop() = 0;

protected:
    int port;
    ThreadPool& threadPool;
    MessageProcessor& messageProcessor;
};

#endif
