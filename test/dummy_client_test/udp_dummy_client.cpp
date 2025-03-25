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
#include <sys/epoll.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024
#define NUM_CLIENTS 500

using namespace std;
using namespace rapidjson;

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

void logError(const string& logMessage) {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);

    stringstream fileNameStream;
    fileNameStream << put_time(localTime, "%Y-%m-%d") << ".log";
    string fileName = fileNameStream.str();

    ofstream logFile(fileName, ios::app);
    if (logFile.is_open()) {
        logFile << put_time(localTime, "%H:%M:%S") << " " << logMessage << endl;
        logFile.close();
    }
}

void handleError(const string& errorMessage) {
    stringstream logStream;
    logStream << "Error: " << errorMessage;
    logError(logStream.str());
    cerr << logStream.str() << endl;
}

void sendMessage(int socket, SA_IN& serverAddr, const string& type, const string& content) {
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    writer.StartObject();
    writer.Key("type");
    writer.String(type.c_str());
    writer.Key("content");
    writer.StartObject();
    writer.Key("message");
    writer.String(content.c_str());
    writer.EndObject();
    writer.EndObject();

    string jsonMessage = buffer.GetString();
    sendto(socket, jsonMessage.c_str(), jsonMessage.size(), 0, (SA*)&serverAddr, sizeof(serverAddr));
}

string receiveMessage(int socket, SA_IN& serverAddr) {
    char buffer[BUFFER_SIZE] = {0};
    socklen_t addrLen = sizeof(serverAddr);
    ssize_t received = recvfrom(socket, buffer, BUFFER_SIZE, 0, (SA*)&serverAddr, &addrLen);
    if (received <= 0) {
        handleError("Failed to receive message");
        return "";
    }
    buffer[received] = '\0';

    Document document;
    document.Parse(buffer);
    if (!document.IsObject()) {
        handleError("Invalid JSON format");
        return "";
    }

    string type = document["type"].GetString();
    string content = document["content"]["message"].GetString();

    cout << "Received JSON Message - Type: " << type << ", Content: " << content << endl;
    return type + ":" + content;
}

void runClient(int clientId) {
    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        handleError("Socket creation failed");
        return;
    }

    SA_IN serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        handleError("Invalid address or address not supported");
        close(clientSocket);
        return;
    }

    cout << "Client " << clientId << " started." << endl;

    vector<string> messageTypes = {"ECHO", "CHAT", "LOGIN", "TASK", "DELAY"};
    srand(time(nullptr) + clientId);

    while (true) {
        int randomIndex = rand() % messageTypes.size();
        sendMessage(clientSocket, serverAddr, messageTypes[randomIndex], "Request");
        receiveMessage(clientSocket, serverAddr);
        this_thread::sleep_for(chrono::milliseconds(rand() % 1001));
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
