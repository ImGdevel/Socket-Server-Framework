#include "Server.h"
#include "Logger.h"
#include <memory>
#include <stdexcept>
#include "handler/IEventHandler.h"
#include "handler/TestJSONEventHandler.h"
#include "messages/parser/JsonParser.h"
#include "dispatcher/EventRegistry.h"

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
    auto md = make_unique<MessageDispatcher>(move(eventRegistry));

    return unique_ptr<Server>(new Server(port, workerCount, move(tp), move(md)));
}

// Server 생성자
Server::Server(int port, int workerCount, unique_ptr<ThreadPool> tp, unique_ptr<MessageDispatcher> md)
    : port(port), workerCount(workerCount), threadPool(move(tp)), messageDispatcher(move(md)) {
    initialize();
}

// Server 초기화
void Server::initialize() {
    reactor = make_unique<Reactor>(port, *threadPool, *messageDispatcher);
    Logger::debug("Server instance created with port");
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
