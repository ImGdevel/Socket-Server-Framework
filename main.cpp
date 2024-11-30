#include "src/Server.h"
#include <iostream>
#include <string.h>
#include <signal.h>

using namespace std;

static int port = 8080;
static int workerCount = 10;

void configureParameters(int argc, char* argv[]);
void serverShutdownHandler(int sig);

int main(int argc, char* argv[]) {
    try {
        
        configureParameters(argc, argv);

        Server* server = Server::getInstance(port, workerCount);
        
        signal(SIGINT, serverShutdownHandler);

        server->run();

    } catch (const exception& ex) {
        cerr << "[Error]: " << ex.what() << endl;
        return 1;
    }
    return 0;
}

// 서버 파라미터 설정
void configureParameters(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "--port=", 7) == 0) {
            port = stoi(argv[i] + 7);
        } else if (strncmp(argv[i], "--worker=", 9) == 0) {
            workerCount = stoi(argv[i] + 9);
        }
    }
}

// 서버 강제 종료 인터럽트 핸들러
void serverShutdownHandler(int sig) {
    Server* server = Server::getInstance(0, 0);
    if (server != nullptr) {
        server->terminate();
    }
    exit(0);
}
