#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <vector>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024
#define NUM_CLIENTS 20  // 클라이언트 수

using namespace std;

void errorExit(const std::string& message) {
    perror(message.c_str());
    exit(EXIT_FAILURE);
}

// 정확한 크기의 데이터를 송신
ssize_t sendAll(int socket, const char* buffer, size_t length) {
    size_t totalSent = 0;
    while (totalSent < length) {
        ssize_t sent = send(socket, buffer + totalSent, length - totalSent, 0);
        if (sent <= 0) {
            return sent; // 오류 발생
        }
        totalSent += sent;
    }
    return totalSent;
}

// 정확한 크기의 데이터를 수신
ssize_t recvAll(int socket, char* buffer, size_t length) {
    size_t totalReceived = 0;
    while (totalReceived < length) {
        ssize_t received = recv(socket, buffer + totalReceived, length - totalReceived, 0);
        if (received <= 0) {
            return received; // 오류 발생
        }
        totalReceived += received;
    }
    return totalReceived;
}

// 무작위 메시지 생성
std::string generateRandomMessage() {
    const std::string types[] = {
        "ECHO",
        "LOGIN",
        "CHAT",
        "TASK",
        "DELAY"
    };
    std::string type = types[std::rand() % 5];

    const std::string contents[] = {
        "Hello! This is Echo Message",
        "How are you?",
        "Random test message",
        "Networking is fun!",
        "Let's send data",
        "Echo server is working fine"
    };
    std::string content = contents[std::rand() % 6];
    return type + ":" + content;
}

// 클라이언트 실행 함수
void runClient(int clientId) {
    // 랜덤 메시지 생성을 위한 시드 초기화
    std::srand(std::time(nullptr) + clientId);  // 각 클라이언트에 대해 다른 시드 사용

    // 1. 소켓 생성
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        errorExit("Socket creation failed");
    }

    // 2. 서버 주소 설정
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        errorExit("Invalid address or address not supported");
    }

    // 3. 클라이언트 소켓에 로컬 포트 바인딩
    sockaddr_in clientAddr{};
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = INADDR_ANY;
    clientAddr.sin_port = htons(0);  // 운영체제가 자동으로 포트 할당

    if (bind(clientSocket, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) < 0) {
        errorExit("Bind failed");
    }

    // 4. 서버에 연결
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        errorExit("Connection to server failed");
    }
    std::cout << "Client " << clientId << " connected to server at " << SERVER_IP << ":" << SERVER_PORT << std::endl;

    // 5. 메시지를 1초마다 지속적으로 전송
    while (true) {
        std::string message = generateRandomMessage();
        uint32_t messageLength = htonl(message.size()); // 네트워크 바이트 순서로 변환

        // 송신 데이터 준비: [길이 헤더(4바이트)] + [메시지]
        char sendBuffer[BUFFER_SIZE] = {0};
        memcpy(sendBuffer, &messageLength, sizeof(messageLength)); // 헤더 복사
        memcpy(sendBuffer + sizeof(messageLength), message.c_str(), message.size()); // 메시지 복사

        size_t totalMessageSize = sizeof(messageLength) + message.size();
        if (sendAll(clientSocket, sendBuffer, totalMessageSize) < 0) {
            errorExit("Failed to send message to server");
        }
        std::cout << "Client " << clientId << " sent message: " << message << std::endl;

        // 서버로부터 응답 수신
        uint32_t receivedMessageLength = 0;
        if (recvAll(clientSocket, (char*)&receivedMessageLength, sizeof(receivedMessageLength)) <= 0) {
            errorExit("Failed to receive message length from server");
        }
        receivedMessageLength = ntohl(receivedMessageLength); // 호스트 바이트 순서로 변환

        // 메시지 수신
        if (receivedMessageLength > BUFFER_SIZE - 1) {
            errorExit("Received message is too large");
        }

        char receiveBuffer[BUFFER_SIZE] = {0};
        if (recvAll(clientSocket, receiveBuffer, receivedMessageLength) <= 0) {
            errorExit("Failed to receive message from server");
        }
        std::cout << "Client " << clientId << " received message: " << receiveBuffer << std::endl;

        // 1초 대기
        sleep(1);
    }

    // 6. 소켓 닫기
    close(clientSocket);
    std::cout << "Client " << clientId << " connection closed" << std::endl;
}

int main() {
    // 클라이언트 실행 스레드 생성
    std::vector<std::thread> clientThreads;

    // 여러 개의 클라이언트를 실행
    for (int i = 0; i < NUM_CLIENTS; ++i) {
        clientThreads.push_back(std::thread(runClient, i + 1));
    }

    // 모든 클라이언트 스레드가 끝날 때까지 대기
    for (auto& t : clientThreads) {
        t.join();
    }

    return 0;
}
