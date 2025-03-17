#include "server/Server.h"
#include "server/ServerConfig.h"
#include "Logger.h"
#include <iostream>
#include <csignal>
#include <memory>

using namespace std;

void signalHandler(int sig) {
    Logger::info("Received termination signal. Shutting down server.");
    exit(EXIT_SUCCESS);
}

int main() {
    signal(SIGINT, signalHandler);
    
    auto server = Server::Builder()
        .setPort(ServerConfig::getPort())
        .setWorkerCount(ServerConfig::getWorkerCount())
        .build();
    
    server->run();
    return 0;
}
