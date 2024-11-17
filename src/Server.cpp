#include "Server.h"
#include <signal.h>
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

Server::Server(int port, int worker ) : isRunning(true) {

    // todo : reactor와 Thread pool 세팅

    signal(SIGINT, [](int) { cout << "Shutting down server...\n"; });
}

Server::~Server(){
    delete reactor;
    delete threadPool;
}

void Server::run() {
    cout << "Server running on port " << port << "...\n";

    // reactor 실행
}
