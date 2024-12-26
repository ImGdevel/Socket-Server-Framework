#include "Server.h"
#include "Logger.h"
#include "dispatcher/MessageDispatcherFactory.h"
#include <memory>

using namespace std;

Server& Server::getInstance(int port, int workerCount) {
    static Server instance(port, workerCount);
    return instance;
}

Server::Server(int port, int workerCount)
    : port(port), workerCount(workerCount) {
    initialize();
}

void Server::initialize() {

    threadPool = make_unique<ThreadPool>(workerCount);
    messageDispatcher = MessageDispatcherFactory::createDispatcher("json-rapid");
    reactor = make_unique<Reactor>(port, *threadPool, *messageDispatcher);

    Logger::debug("Server instance created");
}

Server::~Server() {
    terminate();
}

void Server::run() {
    Logger::info("Server is starting on port " + to_string(port) + " with " + to_string(workerCount) + " workers.");
    reactor->start();
}

void Server::terminate() {
    if (reactor != nullptr) {
        reactor->stop();
        reactor = nullptr;
        Logger::info("Reactor stopped.");
    }
    if (threadPool != nullptr) {
        threadPool = nullptr;
        Logger::info("Thread pool shutdown.");
    }
    Logger::info("Server shutdown.");
}
