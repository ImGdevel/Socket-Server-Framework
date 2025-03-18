#include "Server.h"
#include "Logger.h"
#include <memory>
#include "messages/parser/JsonParser.h"
#include "handler/MyEventHandler.h"
#include "dispatcher/MessageDispatcher.h"

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

unique_ptr<Server> Server::Builder::build() {
    return unique_ptr<Server>(new Server(port, workerCount));
}

// Server 생성자
Server::Server(int port, int workerCount)
    : port(port), workerCount(workerCount) {
    initialize();
}

// Server 초기화
void Server::initialize() {
    threadPool = make_unique<ThreadPool>(workerCount);

    eventRegistry = make_unique<EventRegistry>();

    auto parser = make_unique<JSONParser>();

    messageDispatcher = make_unique<MessageDispatcher>(move(parser), move(eventRegistry));

    reactor = make_unique<Reactor>(port, *threadPool, *messageDispatcher);

    MyEventHandler handler;
    eventRegistry->registerEvents(handler);
    
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
