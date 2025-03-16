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
#include <fstream>
#include <iomanip>
#include <sstream>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024
#define NUM_CLIENTS 20

using namespace std;

void logError(const string& logMessage) {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);

    stringstream fileNameStream;
    fileNameStream << put_time(localTime, "%Y-%m-%d") << "-log.txt";
    string fileName = fileNameStream.str();

    ofstream logFile(fileName, ios::app);
    if (logFile.is_open()) {
        logFile << put_time(localTime, "%H:%M:%S") << " " << logMessage << endl;
        logFile.close();
    }
}

void handleError(const string& errorMessage, int socket) {
    stringstream logStream;
    logStream << "Socket " << socket << " Error: " << errorMessage;
    logError(logStream.str());
    cerr << logStream.str() << endl;
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
bool sendMessage(int socket, const string& message) {
    try {
        uint32_t messageLength = htonl(message.size());
        char buffer[BUFFER_SIZE] = {0};
        memcpy(buffer, &messageLength, sizeof(messageLength));
        memcpy(buffer + sizeof(messageLength), message.c_str(), message.size());

        if (sendAll(socket, buffer, sizeof(messageLength) + message.size()) < 0) {
            throw runtime_error("Failed to send message");
        }
        return true;
    } catch (const exception& e) {
        handleError(e.what(), socket);
        return false;
    }
}

// 메시지 수신 로직
string receiveMessage(int socket) {
    try {
        uint32_t receivedMessageLength = 0;
        if (recvAll(socket, (char*)&receivedMessageLength, sizeof(receivedMessageLength)) <= 0) {
            throw runtime_error("Failed to receive message length");
        }
        receivedMessageLength = ntohl(receivedMessageLength);

        if (receivedMessageLength > BUFFER_SIZE - 1) {
            throw runtime_error("Received message is too large");
        }

        char response[BUFFER_SIZE] = {0};
        if (recvAll(socket, response, receivedMessageLength) <= 0) {
            throw runtime_error("Failed to receive message");
        }

        return string(response, receivedMessageLength);
    } catch (const exception& e) {
        handleError(e.what(), socket);
        return "";
    }
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

// 커스텀 흐름 핸들러를 위한 타입 정의
using REQHandler = function<void(int)>;

void handleEcho(int socket) {
    if (sendMessage(socket, "ECHO:")) {
        string response = receiveMessage(socket);
        if (!response.empty()) {
            cout << "ECHO Response: " << response << endl;
        }
    }
}

void handleChat(int socket) {
    if (sendMessage(socket, "CHAT:")) {
        cout << "CHAT handler executed." << endl;
    }
}

void handleLogin(int socket) {
    if (sendMessage(socket, "LOGIN:")) {
        cout << "LOGIN handler executed." << endl;
    }
}

void handleTask(int socket) {
    if (sendMessage(socket, "TASK:")) {
        cout << "TASK handler executed." << endl;
    }
}

void handleDelay(int socket) {
    if (sendMessage(socket, "DELAY:")) {
        cout << "DELAY handler executed." << endl;
        sleep(2); // 2초 지연
    }
}

void runClient(int clientId) {
    srand(time(nullptr) + clientId);

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        handleError("Socket creation failed", clientSocket);
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        handleError("Invalid address or address not supported", clientSocket);
        close(clientSocket);
        return;
    }

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        handleError("Connection to server failed", clientSocket);
        close(clientSocket);
        return;
    }
    cout << "Client " << clientId << " connected to server." << endl;

    unordered_map<string, REQHandler> eventHandlers;

    eventHandlers["ECHO"] = handleEcho;
    eventHandlers["CHAT"] = handleChat;
    eventHandlers["LOGIN"] = handleLogin;
    eventHandlers["TASK"] = handleTask;
    eventHandlers["DELAY"] = handleDelay;

    while (true) {
        try {
            for (const auto& [type, handler] : eventHandlers) {
                handler(clientSocket);
                sleep(1);
            }
        } catch (const exception& e) {
            handleError(e.what(), clientSocket);
        }
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
