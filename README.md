
English | [한국어](docs/README_kr.md)

## Socket Server Framework

This project started as an improvement of the existing [prototype server](https://github.com/ImGdevel/MJU-backend2024/tree/main/chat_server) with the goal of building a high-performance network server. The project applies an **epoll-based asynchronous I/O + thread pool architecture** to achieve high concurrency performance.

> ### Development Background
> This project was initiated with the goal of building a high-performance network server. Initially, it started with a prototype server built during my undergraduate course. However, the server had a simple architecture and lacked performance and stability for real-world applications.
> As a result, I wanted to develop a general-purpose socket server framework that could be used in actual services, not just for learning purposes.
> I focused on scalability, performance, and stability to design a system that can be used in game servers and handle large-scale traffic efficiently.

<br>

---

## Key Goals
- **High Performance & Scalability** – Utilizes epoll-based asynchronous I/O and thread pool architecture to handle tens of thousands of simultaneous connections reliably.
- **Modular Design** – Capable of supporting database integration, multi-reactor patterns, and various APIs and message formats (JSON, Protobuf, etc.).
- **Game Server Optimization** – Supports both TCP/UDP, low-latency packet processing, and customizable protocol extensions.
- **Production-Ready** – Designed for real-world service applications, not just for learning purposes.

This project is not just a basic socket server; it aims to be a general-purpose network server framework for games and other real-time services.

<br>

## Project Structure

You can check the project structure here.

<br>

---

## Getting Started

### 1. Requirements
   
To build and run the project, the following packages are required:
- A C++17 compatible compiler (e.g., g++)
- CMake (for building Google Test)
- Make (for building the project)

For Linux, you can install them as follows:
```shell
sudo apt update
sudo apt install -y g++ cmake make
```

### 2. Clone the Project and Build

```shell
git clone https://github.com/your-repo/socket-server-framework.git
cd socket-server-framework
make
```

### 3. Run the Server

```shell
./build/server
```

<br>

## Running with Docker

### 1. Build the Docker Image
```shell
docker build -t socket-server .
```

### 2. Run the Docker Container
```shell
docker run --rm -p 8080:8080 socket-server
```
  | By default, the server uses port 8080. You can change the port with `-p <host-port>:<container-port>` if needed.

<br>

## Additional Commands

```
make download // Install external dependencies locally
make test  // Run tests
make clean // Remove build artifacts
make clean-all // Remove build artifacts and external dependencies
```

---
