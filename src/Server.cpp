#include "Server.h"
#include <iostream>
#include <memory>

using namespace std;

Server* Server::instance = nullptr;

void echoHandler(shared_ptr<ClientSession> session, const string& message) {
    if (session) {
        string echoMessage = "Echo: " + message;
        cout << "send message : " << echoMessage << endl;
        session->sendMessage(echoMessage);
    }
}

Server::Server(int port, int workerCount) : port(port), workerCount(workerCount) {

    eventHandler.registerHandler("echo", echoHandler);

    threadPool = make_unique<ThreadPool>(workerCount);
    reactor = make_unique<Reactor>(port, *threadPool, eventHandler);
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
    std::cout << "Server is starting on port " << port 
                << " with " << workerCount << " workers." << std::endl;

    reactor->start();
}

void Server::terminate(){
    if(reactor != nullptr){
        reactor->stop();
        reactor = nullptr;
    }
    if (threadPool != nullptr) {
        threadPool = nullptr;
    }
    cout << "Server shutdown" << endl;
}

