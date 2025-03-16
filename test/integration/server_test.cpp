#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "../../src/server/Server.h"
#include "../../src/server/reactor/Reactor.h"
#include <iostream>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

class ServerIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 서버를 별도 스레드에서 실행
        serverThread = std::thread([]() {
            Server& server = Server::getInstance(SERVER_PORT, 4);
            server.run();
        });

        // 서버가 실행될 시간을 확보
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void TearDown() override {
        // 서버 종료
        Server::getInstance(SERVER_PORT, 4).terminate();
        std::cerr << "종료 시도" << std::endl;
        if (serverThread.joinable()) {
            serverThread.join();
        }
        std::cerr << "test 서버 정리 완료" << std::endl;
    }

    std::thread serverThread;
};

// 클라이언트 소켓 생성 및 서버 연결 테스트
TEST_F(ServerIntegrationTest, ClientCanConnectToServer) {
    
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_NE(clientSocket, -1) << "Failed to create client socket";

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    int connection = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    ASSERT_EQ(connection, 0) << "Client failed to connect to server";

    close(clientSocket);
}

// 클라이언트가 서버에 메시지를 보내고 응답을 받는지 테스트
TEST_F(ServerIntegrationTest, ClientCanSendAndReceiveMessage) {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_NE(clientSocket, -1);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    int connection = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    ASSERT_EQ(connection, 0);

    std::string message = "ECHO: Hello Server";
    send(clientSocket, message.c_str(), message.size(), 0);

    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    ASSERT_GT(bytesRead, 0) << "Did not receive response from server";

    std::string response(buffer, bytesRead);
    EXPECT_EQ(response, "ECHO: Hello Server") << "Server response mismatch";

    close(clientSocket);
}

// 여러 클라이언트 동시 연결 테스트
TEST_F(ServerIntegrationTest, MultipleClientsCanConnect) {
    const int numClients = 10;
    std::vector<int> clientSockets(numClients);

    for (int i = 0; i < numClients; ++i) {
        clientSockets[i] = socket(AF_INET, SOCK_STREAM, 0);
        ASSERT_NE(clientSockets[i], -1);

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(SERVER_PORT);
        inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

        int connection = connect(clientSockets[i], (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        ASSERT_EQ(connection, 0);
    }

    for (int i = 0; i < numClients; ++i) {
        close(clientSockets[i]);
    }
}

// 부하 테스트: 다수의 클라이언트가 동시에 요청을 보냄
TEST_F(ServerIntegrationTest, StressTestWithMultipleClients) {
    const int numClients = 50;
    std::vector<std::thread> clientThreads;

    for (int i = 0; i < numClients; ++i) {
        clientThreads.emplace_back([]() {
            int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (clientSocket < 0) return;

            sockaddr_in serverAddr{};
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(SERVER_PORT);
            inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

            if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == 0) {
                std::string message = "ECHO: Hello Stress Test";
                send(clientSocket, message.c_str(), message.size(), 0);

                char buffer[BUFFER_SIZE] = {0};
                recv(clientSocket, buffer, sizeof(buffer), 0);
            }

            close(clientSocket);
        });
    }

    for (auto& t : clientThreads) {
        t.join();
    }
}
