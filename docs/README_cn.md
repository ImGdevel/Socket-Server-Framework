![소캣 서버 아키택쳐-페이지-3 drawio (1)](https://github.com/user-attachments/assets/d7dcb3db-2a14-46c4-9a90-3bcb58166379)

🇺🇸 [English](https://github.com/ImGdevel/Custom-Socket-Server) /
🇰🇷 [Korean](https://github.com/ImGdevel/Socket-Server-Framework/blob/main/docs/README_kr.md) /
🇨🇳 中文 /
🇪🇸 [Español](https://github.com/ImGdevel/Socket-Server-Framework/blob/main/docs/README_es.md)

### 本项目是一个基于 Reactor-Worker 模式的 Socket 服务器框架，利用基于 epoll 的异步 I/O 和线程池架构。只需简单配置，即可用于聊天服务器、游戏服务器、分布式系统等多种服务。

> ### 核心目标
> - **高性能 & 可扩展性** – 采用基于 epoll 的异步 I/O 和线程池，高效处理数万个并发连接。
> - **模块化设计** – 支持数据库集成、多 Reactor 模式，以及多种 API 和消息格式（JSON、Protobuf 等）。
> - **游戏服务器优化** – 同时支持 TCP 和 UDP，低延迟数据包处理，可扩展自定义协议。
> - **可用于生产环境** – 不是简单的演示项目，而是专为实际服务部署设计。

本项目的目标是提供一个通用的网络服务器框架，不仅限于简单的 Socket 服务器，还可用于游戏和其他实时应用服务。

<br>

## 🛠 技术栈
- **Reactor 模式（I/O 多路复用、非阻塞 I/O）**
- **基于消息队列的异步处理**
- **可扩展的事件处理器（支持自定义事件处理）**
- **基于 Filter Chain 的消息预处理（支持自定义过滤器）**
- **与 Redis、数据库和消息代理集成**

## 📂 服务器架构

1. **客户端 → 负载均衡器**：负载均衡器将多个客户端请求分发到 `Reactor`。
2. **Reactor（生产者）→ 消息队列**：`Reactor` 异步处理客户端请求，并将其存储到消息队列。
3. **消息队列 → Worker（消费者）**：`Worker` 从队列中获取消息，并使用 `FilterChain` 进行处理。
4. **消息分发器 → 事件处理器**：将消息分发到相应的处理器进行最终处理，并与 `Redis`、数据库和 `MessageBroker` 交互。

<br>

---

## 🏗️ 快速开始

### 1️⃣ 安装必需的软件包
```sh
sudo apt update
sudo apt install -y g++ cmake make
```

### 2️⃣ 克隆并构建项目
```sh
git clone https://github.com/your-repo/socket-server-framework.git
cd socket-server-framework
make
```

### 3️⃣ 运行服务器
```sh
./build/server
```

## 🛠️ 自定义服务器配置

### 1️⃣ 定义事件处理器
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

### 2️⃣ 配置并运行服务器
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

### 3️⃣ 构建 & 运行
```sh
make
./build/server
```

## 🐳 使用 Docker 运行

### 1️⃣ 构建 Docker 镜像
```sh
docker build -t socket-server .
```

### 2️⃣ 运行容器
```sh
docker run --rm -p 8080:8080 socket-server
```
默认情况下，服务器运行在 **8080 端口**。可以使用 `-p <主机端口>:<容器端口>` 进行修改。

## 🔧 额外命令
```sh
make download  # 安装依赖
make test      # 运行测试
make clean     # 删除构建文件
make clean-all # 删除构建文件和依赖项
```

<br>

---

## 📜 许可证

本项目遵循开源许可证。

- **教育用途**：完全免费
- **个人及小型初创企业使用**：适用 MIT 许可证
- **企业及商业用途**：需单独授权

详情请参阅 LICENSE 文件。

## 📜 贡献

欢迎提交 PR 以改进本项目！
如有问题或建议，请访问 [GitHub Issues](https://github.com/your-repo/socket-server-framework/issues) 提交反馈。
'''

