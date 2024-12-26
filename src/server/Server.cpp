#include "Server.h"
#include "Logger.h"
#include "dispatcher/HandlerConfigurator.h"
#include "handler/TestEventHandler.h"
#include "handler/TestJSONEventHandler.h"
#include "messages/parser/StringParser.h"
#include "messages/parser/JsonParser.h"
#include <memory>

using namespace std;

Server& Server::getInstance(int port, int workerCount) {
    static Server instance(port, workerCount);
    return instance;
}

Server::Server(int port, int workerCount)
    : port(port), workerCount(workerCount), messageDispatcher(nullptr) {
    initialize();
}

void Server::initialize() {
    auto parser = make_unique<JSONParserRapid>();
    messageDispatcher = std::make_unique<MessageDispatcher>(std::move(parser));

    TestJSONEventHandler handler;
    HandlerConfigurator::registerHandlers(*messageDispatcher, handler);

    threadPool = make_unique<ThreadPool>(workerCount);
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
