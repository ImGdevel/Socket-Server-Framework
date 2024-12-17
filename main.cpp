#include "src/Server.h"
#include <iostream>
#include <string>
#include <cstring>
#include <csignal>
#include <cstdlib>
#include <stdexcept>

using namespace std;

constexpr int DEFAULT_PORT = 8080;
constexpr int DEFAULT_WORKER_COUNT = 10;

static int port = DEFAULT_PORT;
static int workerCount = DEFAULT_WORKER_COUNT;

void configureParameters(int argc, char* argv[]);
void serverShutdownHandler(int sig);

int main(int argc, char* argv[]) {
    try {
        signal(SIGINT, serverShutdownHandler);
        configureParameters(argc, argv);
        Server* server = Server::getInstance(port, workerCount);
        server->run();

    } catch (const exception& ex) {
        cerr << "[Error]: " << ex.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

// 서버 파라미터 설정
void configureParameters(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        try {
            if (strcmp(argv[i], "--port") == 0 && i + 1 < argc) {
                port = stoi(argv[++i]);
            } 
            else if (strcmp(argv[i], "--worker") == 0 && i + 1 < argc) {
                workerCount = stoi(argv[++i]);
            } 
            else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
                port = stoi(argv[++i]);
            } 
            else if (strcmp(argv[i], "-w") == 0 && i + 1 < argc) {
                workerCount = stoi(argv[++i]);
            }
            else {
                throw invalid_argument(string("Unsupported option: ") + argv[i]);
            }
        } catch (const invalid_argument& e) {
            cerr << "[Error]: Invalid value for option " << argv[i - 1] 
                 << ". Expected a number." << endl;
            exit(EXIT_FAILURE);
        } catch (const out_of_range& e) {
            cerr << "[Error]: Value out of range for option " << argv[i - 1] << endl;
            exit(EXIT_FAILURE);
        }
    }
}

// 서버 강제 종료 인터럽트 핸들러
void serverShutdownHandler(int sig) {
    Server* server = Server::getInstance(0, 0);
    if (server != nullptr) {
        server->terminate();
    }
    exit(EXIT_SUCCESS);
}
