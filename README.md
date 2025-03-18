![소캣 서버 아키택쳐-페이지-3 drawio (1)](https://github.com/user-attachments/assets/d7dcb3db-2a14-46c4-9a90-3bcb58166379)

🇺🇸 English /
🇰🇷 [한국어](https://github.com/ImGdevel/Socket-Server-Framework/blob/main/docs/README_kr.md) /
🇨🇳 [中文](https://github.com/ImGdevel/Socket-Server-Framework/blob/main/docs/README_cn.md) /
🇪🇸 [Español](https://github.com/ImGdevel/Socket-Server-Framework/blob/main/docs/README_es.md)

### This project is a socket server framework based on the Reactor-Worker pattern, utilizing epoll-based asynchronous I/O and a thread pool architecture. With minimal configuration, it can be used for chat servers, game servers, distributed systems, and more.

> ### Key Objectives
> - **High Performance & Scalability** – Handles tens of thousands of concurrent connections efficiently using epoll-based asynchronous I/O and a thread pool.
> - **Modular Design** – Supports database integration, multi-reactor patterns, and various API and message formats (JSON, Protobuf, etc.).
> - **Optimized for Game Servers** – Supports both TCP and UDP, low-latency packet processing, and custom protocol extensions.
> - **Production-Ready** – Designed for real-world deployment, not just a simple demonstration project.

This project aims to be a general-purpose network server framework, not just a simple socket server, enabling real-time applications like games and other services.

<br>

--- 

## 🛠 Technology Stack
- **Reactor Pattern (I/O Multiplexing, Non-blocking I/O)**
- **Asynchronous Processing with Message Queue**
- **Extensible Event Handler (Supports Custom Event Processing)**
- **Message Preprocessing with Filter Chain (Supports Custom Filters)**
- **Integration with Redis, Database, and Message Broker**

<br>


## 📂 Server Architecture

![소캣 서버 아키택쳐-페이지-2 drawio](https://github.com/user-attachments/assets/bc8727bb-dbf8-4433-9099-5108b600afbf)

- **Client → Load Balancer**  
   - The load balancer distributes multiple client requests to the `Reactor`.

- **Reactor (Producer) → Message Queue**  
   - The `Reactor` asynchronously processes client requests and stores them in the message queue.
- **Message Queue → Worker (Consumer)**  
   - The `Worker` retrieves messages from the queue and processes them.
   - Message preprocessing is done via `FilterChain`.
- **Message Dispatcher → Event Handler**  
   - Messages are forwarded to the appropriate handler for final processing.
   - Interacts with `Redis`, `Database`, and `MessageBroker`.


<br>

--- 

## 🏗️ Getting Started

### 1️⃣ Install Required Packages
```sh
sudo apt update
sudo apt install -y g++ cmake make
```

### 2️⃣ Clone and Build the Project
```sh
git clone https://github.com/your-repo/socket-server-framework.git
cd socket-server-framework
make
```

### 3️⃣ Run the Server
```sh
./build/server
```

<br>


## 🛠️ Custom Server Configuration

### 1️⃣ Define an Event Handler
```cpp
// CustomEventHandler.h
#include "IEventHandler.h"
#include "ClientRequest.h"
#include "Logger.h"
#include <unordered_map>

class CustomEventHandler : public IEventHandler {
public:
    std::unordered_map<std::string, HandlerFunc> createHandlers() const override {
        return {
            {"LOGIN", [this](const ClientRequest& request) { this->onLogin(request); }}
        };
    }

private:
    void onLogin(const ClientRequest& request) {
        Logger::info("Login: " + request.toString());
    }
};
```

### 2️⃣ Configure and Run the Server
```cpp
// main.cpp
#include "Server.h"
#include "CustomEventHandler.h"

int main() {
   CustomEventHandler handler;
   auto server = Server::Builder()
       .setPort(9090)
       .setWorkerCount(8)
       .setEventHandler(handler)
       .setMessageType("json")
       .build();
    server->run();
}
```

### 3️⃣ Build & Run
```sh
make
./build/server
```

<br>


## 🐳 Running with Docker

### 1️⃣ Build Docker Image
```sh
docker build -t socket-server .
```

### 2️⃣ Run Container
```sh
docker run --rm -p 8080:8080 socket-server
```
By default, the server runs on **port 8080**. You can modify it using `-p <host_port>:<container_port>`.

## 🔧 Additional Commands
```sh
make download  # Install dependencies
make test      # Run tests
make clean     # Remove build files
make clean-all # Remove build files and dependencies
```

<br>

--- 

## 📜 License

This project follows an open-source license.

- **Educational Use**: Completely free
- **Personal & Small Startup Use**: MIT License applies
- **Enterprise & Commercial Use**: Requires separate permission

Refer to the LICENSE file for more details.

## 📜 Contributing

We welcome PRs to improve the project!
For inquiries and suggestions, please visit [GitHub Issues](https://github.com/your-repo/socket-server-framework/issues).
'''

