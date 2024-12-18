#include "src/Server.h"
#include "Logger.h"
#include <iostream>
#include <string>
#include <csignal>
#include <cstdlib>
#include <stdexcept>

using namespace std;

constexpr int DEFAULT_PORT = 8080;
constexpr int DEFAULT_WORKER_COUNT = 10;

static int port = DEFAULT_PORT;
static int workerCount = DEFAULT_WORKER_COUNT;

void printHelp();
bool configureParameters(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    try {
        // 서버 종료 시그널
        signal(SIGINT, [](int sig) {
            Server* server = Server::getInstance(0, 0);
            if (server != nullptr) {
                server->terminate();
            }
            exit(EXIT_SUCCESS);
        });

        // 서버 파라미터
        if (!configureParameters(argc, argv)) {
            return EXIT_FAILURE;
        }

        // 서버 애플리케이션 실행
        Server* server = Server::getInstance(port, workerCount);
        server->run();

    } catch (const exception& ex) {
        Logger::error("[Error]: " + string(ex.what()));
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

// 서버 파라미터 help
void printHelp() {
    cout << "Server Application Help\n\n"
         << "Usage:\n"
         << "  server [--port <port>] [--worker <workerCount>]\n"
         << "  server [-p <port>] [-w <workerCount>]\n"
         << "  server --help\n\n"
         << "Options:\n"
         << "  --port, -p    Specify the port number (default: 8080).\n"
         << "  --worker, -w  Specify the number of worker threads (default: 10).\n"
         << "  --help        Show this help message and exit.\n"
         << "\n";
}

// 서버 파라미터 설정
bool configureParameters(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        try {
            string arg = argv[i];
            if (arg == "--help") {
                printHelp();
                exit(EXIT_SUCCESS);
            }
            else if (arg == "--port" && i + 1 < argc) {
                port = stoi(argv[++i]);
            } 
            else if (arg == "--worker" && i + 1 < argc) {
                workerCount = stoi(argv[++i]);
            } 
            else if (arg == "-p" && i + 1 < argc) {
                port = stoi(argv[++i]);
            } 
            else if (arg == "-w" && i + 1 < argc) {
                workerCount = stoi(argv[++i]);
            } 
            else {
                Logger::error("[Error]: Unsupported option: " + arg);
                return false;
            }
        } catch (const invalid_argument&) {
            Logger::error("[Error]: Invalid value for option: " + string(argv[i - 1]) + ". Expected a number.");
            return false;
        } catch (const out_of_range&) {
            Logger::error("[Error]: Value out of range for option: " + string(argv[i - 1]));
            return false;
        }
    }
    return true;
}
