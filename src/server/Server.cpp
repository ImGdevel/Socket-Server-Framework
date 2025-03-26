#include "Server.h"
#include "Logger.h"
#include <memory>
#include <stdexcept>
#include "IEventHandler.h"
#include "EventRegistry.h"
#include "DefaultFilter.h"
#include "DefaultFilterX.h"

using namespace std;

void Server::Builder::validate() const {
    if (port <= 0) {
        Logger::error("Port must be greater than 0");
        throw invalid_argument("Port must be greater than 0");
    }
    if (workerCount <= 0) {
        Logger::error("Worker count must be greater than 0");
        throw invalid_argument("Worker count must be greater than 0");
    }
    if (messageDispatcherType.empty()) {
        Logger::error("Message dispatcher type must be set");
        throw invalid_argument("Message dispatcher type must be set");
    }
}

Server::Builder& Server::Builder::setPort(int p) {
    if (p <= 0) {
        Logger::error("Port must be greater than 0");
        throw invalid_argument("Port must be greater than 0");
    }
    port = p;
    return *this;
}

Server::Builder& Server::Builder::setWorkerCount(int count) {
    if (count <= 0) {
        Logger::error("Worker count must be greater than 0");
        throw invalid_argument("Worker count must be greater than 0");
    }
    workerCount = count;
    return *this;
}

Server::Builder& Server::Builder::setMessageType(const string& type) {
    if (type.empty()) {
        Logger::error("Message type cannot be empty");
        throw invalid_argument("Message type cannot be empty");
    }
    messageDispatcherType = type;
    return *this;
}

Server::Builder& Server::Builder::setEventHandler(IEventHandler& handler) {
    eventRegistry->registerHandlers(handler);
    return *this;
}

unique_ptr<Server> Server::Builder::build() {
    validate();

    auto tp = make_unique<ThreadPool>(workerCount);
    auto parserFactory = make_unique<ParserFactory>();
    auto parser = parserFactory->createParser(messageDispatcherType);
    if (!parser) {
        Logger::error("Failed to create parser for type: " + messageDispatcherType);
        throw invalid_argument("Invalid message dispatcher type");
    }

    // todo : 테스트 용 기본 필터 예시 (추후 제거할 것)
    if (filterChain->isEmpty()) {
        Logger::warning("No filters added, using default filter chain");
        filterChain->addFilter(make_unique<DefaultFilter>());
        filterChain->addFilter(make_unique<DefaultFilterX>());
    }

    auto dispatcher = make_unique<MessageDispatcher>(move(eventRegistry));
    auto mp = make_unique<MessageProcessor>(move(dispatcher), move(filterChain), move(parser));

    return unique_ptr<Server>(new Server(port, workerCount, move(tp), move(mp)));
}

// Server 생성자
Server::Server(int port, int workerCount, unique_ptr<ThreadPool> tp, unique_ptr<MessageProcessor> mp)
    : port(port), workerCount(workerCount), threadPool(move(tp)), messageProcessor(move(mp)) {
    initialize();
}

// Server 초기화
void Server::initialize() {
    reactor = make_unique<Reactor>(port, *threadPool, *messageProcessor);
    Logger::debug("Server instance created with port " + to_string(port));
}

Server::~Server() {
    terminate();
}

// Server 실행
void Server::run() {
    Logger::info("Server is starting on port " + to_string(port) + " with " + to_string(workerCount) + " workers");
   reactor->start();
}

// Server 종료
void Server::terminate() {
    if (reactor) {
        reactor->stop();
        reactor.reset();
    }
    if (threadPool) {
        threadPool->stop();
        threadPool.reset();
    }
    Logger::info("Server shutdown completed");
}
