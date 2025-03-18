#include "Server.h"
#include "Logger.h"
#include <memory>
#include "handler/IEventHandler.h"

#include "handler/TestJSONEventHandler.h"
#include "messages/parser/JsonParser.h"
#include "dispatcher/HandlerConfigurator.h"

using namespace std;

// Builder 구현
Server::Builder& Server::Builder::setPort(int p) {
    port = p;
    return *this;
}

Server::Builder& Server::Builder::setWorkerCount(int count) {
    workerCount = count;
    return *this;
}

Server::Builder& Server::Builder::setMessageType(const string& type) {
    messageDispatcherType = type;
    return *this;
}

Server::Builder& Server::Builder::setEventHandler(IEventHandler& handler) {
    eventHandler = &handler;
    return *this;
}

unique_ptr<Server> Server::Builder::build() {
    auto tp = make_unique<ThreadPool>(workerCount);
    auto md = make_unique<MessageDispatcher>();

    TestJSONEventHandler handler;
    HandlerConfigurator::registerHandlers(*md, handler);

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
    Logger::debug("Server instance created");
}

Server::~Server() {
    terminate();
}

// Server 실행
void Server::run() {
    Logger::info("Server is starting on port " + to_string(port) + " with " + to_string(workerCount) + " workers.");
    reactor->start();
}

// Server 종료
void Server::terminate() {
    if (reactor != nullptr) {
        reactor->stop();
        reactor = nullptr;
    }
    if (threadPool != nullptr) {
        threadPool->stop();
        threadPool = nullptr;
    }
    Logger::info("Server shutdown.");
}
