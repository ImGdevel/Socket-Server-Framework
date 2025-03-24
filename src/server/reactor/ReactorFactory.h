#ifndef REACTOR_FACTORY_H
#define REACTOR_FACTORY_H

#include "Reactor.h"
#include "ReactorTCP.h"
#include <string>

class ReactorFactory {
public:
    static std::unique_ptr<Reactor> createReactor(std::string type, int port, ThreadPool& threadPool, MessageProcessor& messageProcessor) {
        // 추후 UDP 추가하기
        return std::make_unique<ReactorTCP>(port, threadPool, messageProcessor);
    }
};

#endif
