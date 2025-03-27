![Socket Server Architecture](https://github.com/user-attachments/assets/d7dcb3db-2a14-46c4-9a90-3bcb58166379)

🇺🇸 [English](https://github.com/ImGdevel/Socket-Server-Framework) /
🇰🇷 [한국어](https://github.com/ImGdevel/Socket-Server-Framework/blob/main/docs/README_kr.md) /
🇨🇳 中文 /
🇪🇸 [Español](https://github.com/ImGdevel/Socket-Server-Framework/blob/main/docs/README_es.md)

# Socket Server Framework

本项目是一个基于 **Reactor-Worker 模式**的**高性能 Socket 服务器框架**，采用**基于 epoll 的异步 I/O** 和**线程池架构**。该框架为构建可扩展的网络应用程序（如聊天服务器、游戏服务器和分布式系统）提供了模块化基础。

## 核心设计原则

- **事件驱动架构** – 基于 Reactor 模式构建，实现高效的 I/O 复用
- **异步处理** – 基于线程池的消息处理实现非阻塞 I/O 操作
- **协议灵活性** – 同时支持 TCP 和 UDP 传输协议
- **消息格式无关** – 支持 JSON、Protobuf 和自定义格式的可插拔解析器
- **生产就绪** – 完善的错误处理、日志记录和资源管理

## 技术架构

![Server Architecture Diagram](https://github.com/user-attachments/assets/bc8727bb-dbf8-4433-9099-5108b600afbf)

### 关键组件

**Reactor 层**
- 针对协议特定事件处理的 `ReactorTCP` 和 `ReactorUDP` 实现
- 基于 epoll 的 I/O 复用实现高效连接管理
- 带有事件通知的非阻塞 Socket 操作

**会话管理**
- 用于连接状态管理的 `ClientSessionTCP` 和 `ClientSessionUDP`
- 协议特定的会话生命周期处理
- 资源清理和连接跟踪

**消息处理管道**
- 用于请求预处理和验证的 `FilterChain`
- 用于将消息路由到适当处理器的 `MessageDispatcher`
- 用于动态处理器注册的 `EventRegistry`

**线程模型**
- 用于可扩展 Worker 管理的 `ThreadPool` 实现
- 带有消息队列的生产者-消费者模式
- 可配置的 Worker 线程数量

## 技术栈

- **语言**: C++17
- **I/O 模型**: epoll (Linux)
- **并发性**: 带有消息队列的线程池
- **消息格式**: JSON (RapidJSON)、Protocol Buffers
- **外部依赖**: Redis (hiredis)、MySQL、Google Test
- **构建系统**: 带有自动化依赖管理的 GNU Make

---

## 快速开始

### 先决条件

```bash
sudo apt update
sudo apt install -y g++ cmake make
```

### 构建和运行

```bash
git clone <repository-url>
cd socket-server-framework
make download  # 安装依赖项
make           # 构建项目
./build/server # 运行服务器
```

服务器使用来自 `ServerConfig` 的默认配置启动，并使用 `TestJSONEventHandler`。

---

## 自定义实现

### 1. 定义事件处理器

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
        Logger::info("用户登录: " + request.toString());
        // 实现逻辑
    }
    
    void handleMessage(const ClientRequest& request) {
        Logger::info("接收到消息: " + request.toString());
        // 实现逻辑
    }
};
```

### 2. 配置服务器

```cpp
#include "Server.h"
#include "CustomEventHandler.h"

int main() {
    CustomEventHandler handler;
    
    auto server = Server::Builder()
        .setPort(9090)
        .setWorkerCount(4)
        .setTransportProtocol("TCP")  // 或 "UDP"
        .setMessageType("json-rapid") // 或 "json", "protobuf"
        .setEventHandler(handler)
        .build();
    
    server->run();
    return 0;
}
```

### 3. 自定义过滤器

```cpp
#include "IFilter.h"

class AuthenticationFilter : public IFilter {
public:
    bool process(ClientRequest& request) override {
        // 认证逻辑
        return true; // 继续处理
    }
};

// 添加到服务器
.addFilter(std::make_unique<AuthenticationFilter>())
```

---

## Docker 部署

### 构建容器

```bash
# 首先构建项目
make

# 构建 Docker 镜像
docker build -t socket-server-framework .
```

### 运行容器

```bash
docker run --rm -p 8080:8080 socket-server-framework
```

---

## 可用命令

```bash
make download   # 下载并构建外部依赖项
make            # 构建服务器可执行文件
make test       # 运行单元测试
make clean      # 移除构建产物
make clean-all  # 移除构建产物和依赖项
```

---

## 项目结构

```
src/
├── main.cpp                    # 应用程序入口点
├── server/
│   ├── Server.h/.cpp          # 使用 Builder 模式的主服务器类
│   ├── ServerConfig.h/.cpp    # 配置管理
│   ├── reactor/               # 事件循环实现
│   │   ├── Reactor.h          # 抽象 Reactor 接口
│   │   ├── ReactorTCP.cpp     # TCP 特定 Reactor
│   │   └── ReactorUDP.cpp     # UDP 特定 Reactor
│   ├── session/               # 连接管理
│   │   ├── ClientSessionTCP.cpp
│   │   └── ClientSessionUDP.cpp
│   ├── threadpool/            # Worker 线程管理
│   ├── dispatcher/            # 消息路由
│   ├── filter/                # 请求预处理
│   ├── handler/               # 事件处理器
│   └── messages/              # 消息解析
└── utils/                     # 实用工具函数
```

---

## 测试

项目使用 Google Test 框架包含全面的单元测试：

```bash
make test
```

测试覆盖范围包括：
- 服务器初始化和配置
- Worker 队列操作
- 客户端会话管理
- 过滤器链处理

---

## 许可证

本项目提供多种许可选项：

- **教育用途**: 免费用于学习和教育目的
- **个人和小型项目**: MIT 许可证
- **企业和商业**: 需要单独的许可协议

详细条款请参阅 LICENSE 文件。

---

## 贡献

我们欢迎贡献来改进框架：

1. Fork 仓库
2. 创建功能分支
3. 实现更改并添加适当的测试
4. 提交拉取请求

如需报告错误和功能请求，请使用 [GitHub Issues](https://github.com/ImGdevel/Socket-Server-Framework/issues)。