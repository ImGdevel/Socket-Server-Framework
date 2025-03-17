#include "server/Server.h"
#include "server/ServerConfig.h"
#include "Logger.h"
#include <iostream>
#include <csignal>
#include <memory>

using namespace std;

static Server* serverInstance; 

void signalHandler(int sig) {
    Logger::debug("Received termination signal. Shutting down server.");

    if (serverInstance) {
        serverInstance->terminate(); 
    }
    Logger::close();
    exit(EXIT_SUCCESS);
}

int main() {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler); 
    
    auto server = Server::Builder()
        .setPort(ServerConfig::getPort())
        .setWorkerCount(ServerConfig::getWorkerCount())
        .build();
    
    serverInstance = server.get(); 
    
    server->run();
    return 0;
}
