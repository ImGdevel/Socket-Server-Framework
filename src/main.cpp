#include "server/Server.h"
#include "server/ServerConfig.h"
#include "Logger.h"
#include <iostream>
#include <csignal>
#include <memory>

#include "server/handler/TestJSONEventHandler.h"

using namespace std;

static Server* serverInstance; 

void signalHandler(int /*sig*/) {
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
    
    TestJSONEventHandler handler;

    auto server = Server::Builder()
        .setPort(ServerConfig::getPort())
        .setWorkerCount(ServerConfig::getWorkerCount())
        .setEventHandler(handler)
        .setMessageType("json-rapid")
        .build();
    
    serverInstance = server.get(); 
    
    server->run();
    return 0;
}
