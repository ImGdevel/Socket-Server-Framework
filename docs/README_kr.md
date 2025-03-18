![소캣 서버 아키택쳐-페이지-3 drawio (1)](https://github.com/user-attachments/assets/d7dcb3db-2a14-46c4-9a90-3bcb58166379)

🇺🇸 [English](https://github.com/ImGdevel/Custom-Socket-Server) /  🇰🇷 한국어 /  🇨🇳 中文

<br>

### 이 프로젝트는 epoll 기반 비동기 I/O + 스레드 풀 구조를 활용한 Reactor-Worker 패턴의 소켓 서버 프레임워크입니다.  간단한 설정만으로 채팅 서버, 게임 서버, 분산 시스템 등 다양한 서비스에 활용할 수 있습니다.


> ### 핵심 목표
> - 고성능 & 확장성 – epoll 기반 비동기 I/O와 스레드 풀 구조를 활용하여 수만 개의 동시 접속을 안정적으로 처리
> - 모듈화된 설계 – 데이터베이스 연동, 멀티 리액터 패턴, 다양한 API 및 메시지 포맷(JSON, Protobuf 등) 지원 가능
> - 게임 서버 최적화 – TCP/UDP 동시 지원, 저지연 패킷 처리, 커스텀 프로토콜 확장 가능
> - 실제 운영 환경 적용 가능 – 단순한 실습용이 아닌, 실제 서비스에 적용할 수 있는 구조

이 프로젝트는 단순한 소켓 서버가 아니라, 게임 및 다양한 실시간 서비스에서 활용할 수 있는 범용 네트워크 서버 프레임워크를 목표로 합니다.

<br>

---

## 🛠 기술 스택
- **Reactor 패턴 (I/O 멀티플렉싱, Non-blocking I/O)**
- **Message Queue 기반 비동기 처리**
- **EventHandler 확장 가능 (사용자 정의 이벤트 처리 지원)**
- **Filter Chain 기반 메시지 전처리 (사용자 정의 Filter 처리 지원)**
- **Redis + Database + Message Broker 연동**

<br>

##  📂 서버 기본 구조

![소캣 서버 아키택쳐-페이지-2 drawio](https://github.com/user-attachments/assets/bc8727bb-dbf8-4433-9099-5108b600afbf)
- **클라이언트 → 로드 밸런서**  
   - 다수의 클라이언트 요청을 로드 밸런서가 `Reactor`에 분배.

- **Reactor (Producer) → Message Queue**  
   - `Reactor`가 클라이언트 요청을 비동기적으로 받아 처리 후 메시지 큐에 저장.
- **Message Queue → Worker (Consumer)**  
   - `Worker`가 큐에서 메시지를 가져와 처리.
   - `FilterChain`을 통해 메시지 전처리.
- **Message Dispatcher → Event Handler**  
   - 메시지를 적절한 핸들러에 전달하여 최종 처리.
   - `Redis`, `Database`, `MessageBroker`와 상호작용.

<br>

---

## 🏗️ 시작하기
### 1️⃣ 필수 설치 패키지
```sh
sudo apt update
sudo apt install -y g++ cmake make
```

### 2️⃣ 프로젝트 클론 및 빌드
```sh
git clone https://github.com/your-repo/socket-server-framework.git
cd socket-server-framework
make
```

### 3️⃣ 실행
```sh
./build/server
```

## 🛠️ 커스텀 서버 구성
### 1️⃣ 이벤트 핸들러 정의
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

### 2️⃣ 서버 설정 및 실행
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

### 3️⃣ 빌드 & 실행
```sh
make
./build/server
```

<br>

## 🐳 Docker로 실행하기
### 1️⃣ Docker 이미지 빌드
```sh
docker build -t socket-server .
```

### 2️⃣ 컨테이너 실행
```sh
docker run --rm -p 8080:8080 socket-server
```
기본적으로 **8080 포트**를 사용합니다. 필요에 따라 `-p <호스트포트>:<컨테이너포트>`로 변경 가능합니다.

## 🔧 추가 명령어
```sh
make download  # 의존성 라이브러리 설치
make test      # 테스트 실행
make clean     # 빌드 결과 삭제
make clean-all # 빌드 및 의존성 삭제
```

<br>

## 📜 라이선스

이 프로젝트는 오픈소스 라이선스를 따릅니다.

- 교육 목적: 완전 무료로 사용 가능

- 개인 사용자 및 소규모 스타트업: MIT 라이선스 적용

- 대기업 및 상업적 이용: 별도 허가 필요

자세한 사항은 LICENSE 파일을 참고하세요.


## 📜 기여하기
프로젝트 개선을 위한 PR을 환영합니다!
문의 및 제안 사항은 [GitHub Issues](https://github.com/your-repo/socket-server-framework/issues)에서 남겨주세요.
'''

