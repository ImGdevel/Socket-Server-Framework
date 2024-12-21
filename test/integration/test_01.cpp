#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <vector>
#include <unordered_map>
#include <functional>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024
#define NUM_CLIENTS 20

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

// 메시지 송신 로직
void sendMessage(int socket, const string& message) {
    uint32_t messageLength = htonl(message.size());
    char buffer[BUFFER_SIZE] = {0};
    memcpy(buffer, &messageLength, sizeof(messageLength));
    memcpy(buffer + sizeof(messageLength), message.c_str(), message.size());

    if (sendAll(socket, buffer, sizeof(messageLength) + message.size()) < 0) {
        errorExit("Failed to send message");
    }
}

// 메시지 수신 로직
string receiveMessage(int socket) {
    uint32_t receivedMessageLength = 0;
    if (recvAll(socket, (char*)&receivedMessageLength, sizeof(receivedMessageLength)) <= 0) {
        errorExit("Failed to receive message length");
    }
    receivedMessageLength = ntohl(receivedMessageLength);

    if (receivedMessageLength > BUFFER_SIZE - 1) {
        errorExit("Received message is too large");
    }

    char response[BUFFER_SIZE] = {0};
    if (recvAll(socket, response, receivedMessageLength) <= 0) {
        errorExit("Failed to receive message");
    }

    return string(response, receivedMessageLength);
}

// 메시지 파싱 유틸리티
pair<string, string> parseMessage(const string& message) {
    size_t delimiterPos = message.find(":");
    if (delimiterPos == string::npos) {
        return {"", ""}; // 잘못된 메시지 형식
    }
    string type = message.substr(0, delimiterPos);
    string content = message.substr(delimiterPos + 1);
    return {type, content};
}

std::string composeMessage(const string& type, const string& content) {
    return type + ":" + content;
}

// 무작위 메시지 타입 생성
std::string generateRandomType() {
    const std::string types[] = {"ECHO", "LOGIN", "CHAT", "TASK", "DELAY"};
    return types[std::rand() % 5];
}

// 무작위 메시지 내용 생성
std::string generateRandomContent() {
    const std::string contents[] = {
        "Hello! This is Echo Message",
        "How are you?",
        "Random test message",
        "Networking is fun!",
        "Let's send data",
        "Echo server is working fine"
    };
    return contents[std::rand() % 6];
}

// 커스텀 흐름 핸들러를 위한 타입 정의
using Handler = function<void(int, const string&)>;

void handleEcho(int socket, const string& message) {
    sendMessage(socket, message);
    string response = receiveMessage(socket);
    cout << "ECHO Response: " << response << endl;
}

void handleChat(int socket, const string& message) {
    cout << "CHAT handler: " << message << endl;
    // 추가 데이터 처리 로직 가능
}

void handleLogin(int socket, const string& message) {
    cout << "LOGIN handler: " << message << endl;
    // 로그인 로직 구현 가능
}

void handleTask(int socket, const string& message) {
    cout << "TASK handler: " << message << endl;
    // 작업 처리 로직 구현 가능
}

void handleDelay(int socket, const string& message) {
    cout << "DELAY handler: " << message << endl;
    sleep(2); // 2초 지연
}

void runClient(int clientId) {
    srand(time(nullptr) + clientId);

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        errorExit("Socket creation failed");
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        errorExit("Invalid address or address not supported");
    }

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        errorExit("Connection to server failed");
    }
    cout << "Client " << clientId << " connected to server." << endl;

    unordered_map<string, Handler> eventHandlers;

    eventHandlers["ECHO"] = handleEcho;
    eventHandlers["CHAT"] = handleChat;
    eventHandlers["LOGIN"] = handleLogin;
    eventHandlers["TASK"] = handleTask;
    eventHandlers["DELAY"] = handleDelay;

    while (true) {
        string type = generateRandomType();
        string content = generateRandomContent();
        string message = composeMessage(type, content);
        auto [parsedType, parsedContent] = parseMessage(message);

        if (eventHandlers.find(parsedType) != eventHandlers.end()) {
            eventHandlers[parsedType](clientSocket, message);
        } else {
            cout << "Unhandled event type: " << parsedType << endl;
        }

        sleep(1);
    }

    close(clientSocket);
}

int main() {
    vector<thread> clientThreads;

    for (int i = 0; i < NUM_CLIENTS; ++i) {
        clientThreads.emplace_back(runClient, i + 1);
    }

    for (auto& t : clientThreads) {
        t.join();
    }

    return 0;
}
