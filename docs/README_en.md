![Socket Server Architecture](https://github.com/user-attachments/assets/d7dcb3db-2a14-46c4-9a90-3bcb58166379)

🇺🇸 English /
🇰🇷 [한국어](https://github.com/ImGdevel/Socket-Server-Framework/blob/main/docs/README_kr.md) /
🇨🇳 [中文](https://github.com/ImGdevel/Socket-Server-Framework/blob/main/docs/README_cn.md) /
🇪🇸 [Español](https://github.com/ImGdevel/Socket-Server-Framework/blob/main/docs/README_es.md)

# Socket Server Framework

This project is a **high-performance socket server framework** implementing the **Reactor-Worker pattern** with **epoll-based asynchronous I/O** and **thread pool architecture**. The framework provides a modular foundation for building scalable network applications such as chat servers, game servers, and distributed systems.

## Core Design Principles

- **Event-Driven Architecture** – Built on the Reactor pattern for efficient I/O multiplexing
- **Asynchronous Processing** – Non-blocking I/O operations with thread pool-based message processing  
- **Protocol Flexibility** – Support for both TCP and UDP transport protocols
- **Message Format Agnostic** – Pluggable parsers for JSON, Protobuf, and custom formats
- **Production-Ready** – Comprehensive error handling, logging, and resource management

## Technical Architecture

![Server Architecture Diagram](https://github.com/user-attachments/assets/bc8727bb-dbf8-4433-9099-5108b600afbf)

### Key Components

**Reactor Layer**
- `ReactorTCP` and `ReactorUDP` implementations for protocol-specific event handling
- epoll-based I/O multiplexing for efficient connection management
- Non-blocking socket operations with event notification

**Session Management**
- `ClientSessionTCP` and `ClientSessionUDP` for connection state management
- Protocol-specific session lifecycle handling
- Resource cleanup and connection tracking

**Message Processing Pipeline**
- `FilterChain` for request preprocessing and validation
- `MessageDispatcher` for routing messages to appropriate handlers
- `EventRegistry` for dynamic handler registration

**Threading Model**
- `ThreadPool` implementation for scalable worker management
- Producer-consumer pattern with message queues
- Configurable worker thread count

## Technology Stack

- **Language**: C++17
- **I/O Model**: epoll (Linux)
- **Concurrency**: Thread pool with message queues
- **Message Formats**: JSON (RapidJSON), Protocol Buffers
- **External Dependencies**: Redis (hiredis), MySQL, Google Test
- **Build System**: GNU Make with automated dependency management

---

## Getting Started

### Prerequisites

```bash
sudo apt update
sudo apt install -y g++ cmake make
```

### Build and Run

```bash
git clone <repository-url>
cd socket-server-framework
make download  # Install dependencies
make           # Build the project
./build/server # Run the server
```

The server starts with the default configuration from `ServerConfig` and uses the `TestJSONEventHandler`.

---

## Custom Implementation

### 1. Define Event Handler

```cpp
#include "IEventHandler.h"
#include "ClientRequest.h"
#include "Logger.h"

class CustomEventHandler : public IEventHandler {
public:
    std::unordered_map<std::string, HandlerFunc> createHandlers() const override {
        return {
            {"LOGIN", [this](const ClientRequest& request) { handleLogin(request); }},
            {"MESSAGE", [this](const ClientRequest& request) { handleMessage(request); }}
        };
    }

private:
    void handleLogin(const ClientRequest& request) {
        Logger::info("User login: " + request.toString());
        // Implementation logic
    }
    
    void handleMessage(const ClientRequest& request) {
        Logger::info("Message received: " + request.toString());
        // Implementation logic
    }
};
```

### 2. Configure Server

```cpp
#include "Server.h"
#include "CustomEventHandler.h"

int main() {
    CustomEventHandler handler;
    
    auto server = Server::Builder()
        .setPort(9090)
        .setWorkerCount(4)
        .setTransportProtocol("TCP")  // or "UDP"
        .setMessageType("json-rapid") // or "json", "protobuf"
        .setEventHandler(handler)
        .build();
    
    server->run();
    return 0;
}
```

### 3. Custom Filters

```cpp
#include "IFilter.h"

class AuthenticationFilter : public IFilter {
public:
    bool process(ClientRequest& request) override {
        // Authentication logic
        return true; // Continue processing
    }
};

// Add to server
.addFilter(std::make_unique<AuthenticationFilter>())
```

---

## Docker Deployment

### Build Container

```bash
# Build the project first
make

# Build Docker image
docker build -t socket-server-framework .
```

### Run Container

```bash
docker run --rm -p 8080:8080 socket-server-framework
```

---

## Available Commands

```bash
make download   # Download and build external dependencies
make            # Build the server executable
make test       # Run unit tests
make clean      # Remove build artifacts
make clean-all  # Remove build artifacts and dependencies
```

---

## Project Structure

```
src/
├── main.cpp                    # Application entry point
├── server/
│   ├── Server.h/.cpp          # Main server class with Builder pattern
│   ├── ServerConfig.h/.cpp    # Configuration management
│   ├── reactor/               # Event loop implementations
│   │   ├── Reactor.h          # Abstract reactor interface
│   │   ├── ReactorTCP.cpp     # TCP-specific reactor
│   │   └── ReactorUDP.cpp     # UDP-specific reactor
│   ├── session/               # Connection management
│   │   ├── ClientSessionTCP.cpp
│   │   └── ClientSessionUDP.cpp
│   ├── threadpool/            # Worker thread management
│   ├── dispatcher/            # Message routing
│   ├── filter/                # Request preprocessing
│   ├── handler/               # Event handlers
│   └── messages/              # Message parsing
└── utils/                     # Utility functions
```

---

## Testing

The project includes comprehensive unit tests using Google Test framework:

```bash
make test
```

Test coverage includes:
- Server initialization and configuration
- Worker queue operations
- Client session management
- Filter chain processing

---

## License

This project is available under multiple licensing options:

- **Educational Use**: Free for learning and educational purposes
- **Personal & Small Projects**: MIT License
- **Enterprise & Commercial**: Requires separate licensing agreement

See the LICENSE file for detailed terms.

---

## Contributing

We welcome contributions to improve the framework:

1. Fork the repository
2. Create a feature branch
3. Implement changes with appropriate tests
4. Submit a pull request

For bug reports and feature requests, please use [GitHub Issues](https://github.com/ImGdevel/Socket-Server-Framework/issues).