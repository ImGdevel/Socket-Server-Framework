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

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024
#define NUM_CLIENTS 500
#define MAX_EVENTS 10

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

ssize_t sendAll(int socket, const char* buffer, size_t length) {
    size_t totalSent = 0;
    while (totalSent < length) {
        ssize_t sent = send(socket, buffer + totalSent, length - totalSent, 0);
        if (sent <= 0) {
            return sent;
        }
        totalSent += sent;
    }
    return totalSent;
}

ssize_t recvAll(int socket, char* buffer, size_t length) {
    size_t totalReceived = 0;
    while (totalReceived < length) {
        ssize_t received = recv(socket, buffer + totalReceived, length - totalReceived, 0);
        if (received <= 0) {
            return received;
        }
        totalReceived += received;
    }
    return totalReceived;
}

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

pair<string, string> parseMessage(const string& message) {
    size_t delimiterPos = message.find(":");
    if (delimiterPos == string::npos) {
        return {"", ""};
    }
    string type = message.substr(0, delimiterPos);
    string content = message.substr(delimiterPos + 1);
    return {type, content};
}

std::string composeMessage(const string& type, const string& content) {
    return type + ":" + content;
}

using Handler = function<void(int)>;

void handleEcho(int socket) {
    if (sendMessage(socket, "ECHO:")) {
        //cout << "ECHO handler executed." << endl;
    }
}

void handleChat(int socket) {
    if (sendMessage(socket, "CHAT:")) {
        //cout << "CHAT handler executed." << endl;
    }
}

void handleLogin(int socket) {
    if (sendMessage(socket, "LOGIN:")) {
        //cout << "LOGIN handler executed." << endl;
    }
}

void handleTask(int socket) {
    if (sendMessage(socket, "TASK:")) {
        //cout << "TASK handler executed." << endl;
    }
}

void handleDelay(int socket) {
    if (sendMessage(socket, "DELAY:")) {
        //cout << "DELAY handler executed." << endl;
        sleep(2);
    }
}

void receiveLoop(int epollFd, unordered_map<int, string>& sockets) {
    epoll_event events[MAX_EVENTS];
    while (true) {
        int numEvents = epoll_wait(epollFd, events, MAX_EVENTS, -1);
        for (int i = 0; i < numEvents; ++i) {
            int clientSocket = events[i].data.fd;
            string message = receiveMessage(clientSocket);
            if (!message.empty()) {
                cout << "Received from socket " << clientSocket << ": " << message << endl;
            } else {
                close(clientSocket);
                sockets.erase(clientSocket);
            }
        }
    }
}

void sendLoop(int socket) {
    static vector<void(*)(int)> handlers = {handleEcho, handleChat, handleLogin, handleTask, handleDelay};

    srand(static_cast<unsigned>(time(nullptr)));

    while (true) {
        int randomIndex = rand() % handlers.size();
        handlers[randomIndex](socket);

        int delay = rand() % 1001;
        this_thread::sleep_for(chrono::milliseconds(delay));
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

    int epollFd = epoll_create1(0);
    if (epollFd < 0) {
        handleError("Failed to create epoll instance", clientSocket);
        close(clientSocket);
        return;
    }

    epoll_event event{};
    event.events = EPOLLIN;
    event.data.fd = clientSocket;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &event) < 0) {
        handleError("Failed to add socket to epoll", clientSocket);
        close(clientSocket);
        close(epollFd);
        return;
    }

    unordered_map<int, string> sockets;
    sockets[clientSocket] = "";

    thread receiveThread(receiveLoop, epollFd, ref(sockets));
    thread sendThread(sendLoop, clientSocket);

    receiveThread.join();
    sendThread.join();

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
