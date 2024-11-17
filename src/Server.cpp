#include "Server.h"
#include "ThreadPool.h"
#include <signal.h>
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

Server::Server(int port, int workerCount ) : isRunning(true) {

    // todo : reactor와 Thread pool 세팅
    threadPool = new ThreadPool(workerCount);
    threadPool->initialize();

    signal(SIGINT, [](int) { cout << "Shutting down server...\n"; });
}

Server::~Server(){
    delete threadPool;
}

void Server::run() {
    cout << "Server running on port " << port << "...\n";

    // reactor 실행
    int n = 0;
    while (n < 100000)
    {
        threadPool->enqueueTask(n);
        n++;
    }
    cout << "종료?? \n";
}
