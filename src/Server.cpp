#include "Server.h"
#include <iostream>

Server::Server(int port, int workerCount) : port(port), workerCount(workerCount) {

}

Server::~Server() {

}

void Server::run() {
    std::cout << "Server is starting on port " << port 
                << " with " << workerCount << " workers." << std::endl;

}
