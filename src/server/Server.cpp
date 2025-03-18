#include "Server.h"
#include "Logger.h"
#include "dispatcher/MessageDispatcherFactory.h"
#include <memory>

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
    messageDispatcher = MessageDispatcherFactory::createDispatcher("json-rapid");
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
