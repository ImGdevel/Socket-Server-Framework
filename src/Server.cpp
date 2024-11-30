#include "Server.h"
#include <iostream>
#include <signal.h>

using namespace std;

Server* Server::instance = nullptr;

Server::Server(int port, int workerCount) : port(port), workerCount(workerCount) {
    reactor = new Reactor(port);
}

Server::~Server() {
    if(reactor != nullptr){
        delete reactor;
    }
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
        delete reactor;
        reactor = nullptr;
    }
    cout << "Server shutdown" << endl;
}