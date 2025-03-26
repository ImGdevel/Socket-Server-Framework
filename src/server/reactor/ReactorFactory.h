#ifndef REACTOR_FACTORY_H
#define REACTOR_FACTORY_H

#include "Reactor.h"
#include "ReactorTCP.h"
#include "ReactorUDP.h"
#include "Logger.h"
#include <string>
#include "TransportProtocol.h"

class ReactorFactory {
public:
    static std::unique_ptr<Reactor> createReactor(TransportProtocol protocol, int port, ThreadPool& threadPool, MessageProcessor& messageProcessor) {
        switch (protocol) {
            case TransportProtocol::TCP:
                return std::make_unique<ReactorTCP>(port, threadPool, messageProcessor);
            case TransportProtocol::UDP:
                return std::make_unique<ReactorUDP>(port, threadPool, messageProcessor);
            default:
                Logger::error("Unsupported transport protocol");
                throw std::invalid_argument("Unsupported transport protocol");
        }
    }
};

#endif
