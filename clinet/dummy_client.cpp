#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

void errorExit(const std::string& message) {
    perror(message.c_str());
    exit(EXIT_FAILURE);
}

int main() {
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

    // 3. 서버에 연결
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        errorExit("Connection to server failed");
    }
    std::cout << "Connected to server at " << SERVER_IP << ":" << SERVER_PORT << std::endl;

    // 4. 서버에 메시지 전송 X 3
    std::string message = "Hello, Server! X 3";
    if (send(clientSocket, message.c_str(), message.size(), 0) < 0) {
        errorExit("Failed to send message to server");
    }
    if (send(clientSocket, message.c_str(), message.size(), 0) < 0) {
        errorExit("Failed to send message to server");
    }
    if (send(clientSocket, message.c_str(), message.size(), 0) < 0) {
        errorExit("Failed to send message to server");
    }
    std::cout << "Message sent: " << message << std::endl;

    // 5. 서버로부터 응답 수신
    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead < 0) {
        errorExit("Failed to receive message from server");
    }
    std::cout << "Message received from server: " << buffer << std::endl;

    // 6. 소켓 닫기
    close(clientSocket);
    std::cout << "Connection closed" << std::endl;

    return 0;
}
