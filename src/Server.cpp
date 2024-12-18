#include "Server.h"
#include "Logger.h"
#include "dispatcher/HandlerConfigurator.h"
#include "handler/EventHandler.h"
#include <iostream>
#include <memory>

using namespace std;

Server* Server::instance = nullptr;

Server::Server(int port, int workerCount) : port(port), workerCount(workerCount) {
    EventHandler handler;
    HandlerConfigurator::registerHandlers(messageDispatcher, handler);

    threadPool = make_unique<ThreadPool>(workerCount);
    reactor = make_unique<Reactor>(port, *threadPool, messageDispatcher);

    Logger::info("Server instance created on port " + to_string(port) + " with " + to_string(workerCount) + " workers.");
}

Server::~Server() {
    terminate();
}

Server* Server::getInstance(int port, int workerCount) {
    if (instance == nullptr) {
        instance = new Server(port, workerCount);
    }
    return instance;
}

void Server::run() {
    Logger::info("Server is starting on port " + to_string(port) + " with " + to_string(workerCount) + " workers.");
    reactor->start();
}

void Server::terminate(){
    if(reactor != nullptr){
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
