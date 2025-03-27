![Socket Server Architecture](https://github.com/user-attachments/assets/d7dcb3db-2a14-46c4-9a90-3bcb58166379)

🇺🇸 [English](https://github.com/ImGdevel/Socket-Server-Framework) / 
🇰🇷 한국어 /
🇨🇳 [中文](https://github.com/ImGdevel/Socket-Server-Framework/blob/main/docs/README_cn.md) /
🇪🇸 [Español](https://github.com/ImGdevel/Socket-Server-Framework/blob/main/docs/README_es.md)

# Socket Server Framework

이 프로젝트는 **epoll 기반 비동기 I/O**와 **Reactor-Worker 패턴**을 사용한 **고성능 소켓 서버 프레임워크**입니다. 채팅 서버, 게임 서버, 분산 시스템 등 확장 가능한 네트워크 애플리케이션 구축을 위한 모듈형 기반을 제공합니다.

## 핵심 설계 원칙

- **이벤트 기반 아키텍처** – 효율적인 I/O 멀티플렉싱을 위한 Reactor 패턴 기반
- **비동기 처리** – 스레드 풀 기반 메시지 처리를 통한 논블로킹 I/O 작업
- **프로토콜 유연성** – TCP 및 UDP 전송 프로토콜 모두 지원
- **메시지 포맷 독립성** – JSON, Protobuf 및 커스텀 포맷용 플러그인 가능한 파서
- **프로덕션 준비** – 포괄적인 오류 처리, 로깅 및 리소스 관리

## 기술 아키텍처

![Server Architecture Diagram](https://github.com/user-attachments/assets/bc8727bb-dbf8-4433-9099-5108b600afbf)

### 주요 구성요소

**Reactor 계층**
- 프로토콜별 이벤트 처리를 위한 `ReactorTCP` 및 `ReactorUDP` 구현
- 효율적인 연결 관리를 위한 epoll 기반 I/O 멀티플렉싱
- 이벤트 알림이 포함된 논블로킹 소켓 작업

**세션 관리**
- 연결 상태 관리를 위한 `ClientSessionTCP` 및 `ClientSessionUDP`
- 프로토콜별 세션 라이프사이클 처리
- 리소스 정리 및 연결 추적

**메시지 처리 파이프라인**
- 요청 전처리 및 검증을 위한 `FilterChain`
- 적절한 핸들러로 메시지 라우팅하는 `MessageDispatcher`
- 동적 핸들러 등록을 위한 `EventRegistry`

**스레딩 모델**
- 확장 가능한 워커 관리를 위한 `ThreadPool` 구현
- 메시지 큐를 사용한 Producer-Consumer 패턴
- 설정 가능한 워커 스레드 수

## 기술 스택

- **언어**: C++17
- **I/O 모델**: epoll (Linux)
- **동시성**: 메시지 큐가 있는 스레드 풀
- **메시지 포맷**: JSON (RapidJSON), Protocol Buffers
- **외부 의존성**: Redis (hiredis), MySQL, Google Test
- **빌드 시스템**: 자동화된 의존성 관리가 포함된 GNU Make

---

## 시작하기

### 필수 사항

```bash
sudo apt update
sudo apt install -y g++ cmake make
```

### 빌드 및 실행

```bash
git clone <repository-url>
cd socket-server-framework
make download  # 의존성 설치
make           # 프로젝트 빌드
./build/server # 서버 실행
```

서버는 `ServerConfig`의 기본 구성으로 시작하며 `TestJSONEventHandler`를 사용합니다.

---

## 커스텀 구현

### 1. 이벤트 핸들러 정의

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
        Logger::info("사용자 로그인: " + request.toString());
        // 구현 로직
    }
    
    void handleMessage(const ClientRequest& request) {
        Logger::info("메시지 수신: " + request.toString());
        // 구현 로직
    }
};
```

### 2. 서버 구성

```cpp
#include "Server.h"
#include "CustomEventHandler.h"

int main() {
    CustomEventHandler handler;
    
    auto server = Server::Builder()
        .setPort(9090)
        .setWorkerCount(4)
        .setTransportProtocol("TCP")  // 또는 "UDP"
        .setMessageType("json-rapid") // 또는 "json", "protobuf"
        .setEventHandler(handler)
        .build();
    
    server->run();
    return 0;
}
```

### 3. 커스텀 필터

```cpp
#include "IFilter.h"

class AuthenticationFilter : public IFilter {
public:
    bool process(ClientRequest& request) override {
        // 인증 로직
        return true; // 처리 계속
    }
};

// 서버에 추가
.addFilter(std::make_unique<AuthenticationFilter>())
```

---

## Docker 배포

### 컨테이너 빌드

```bash
# 먼저 프로젝트 빌드
make

# Docker 이미지 빌드
docker build -t socket-server-framework .
```

### 컨테이너 실행

```bash
docker run --rm -p 8080:8080 socket-server-framework
```

---

## 사용 가능한 명령어

```bash
make download   # 외부 의존성 다운로드 및 빌드
make            # 서버 실행 파일 빌드
make test       # 단위 테스트 실행
make clean      # 빌드 산출물 제거
make clean-all  # 빌드 산출물 및 의존성 제거
```

---

## 프로젝트 구조

```
src/
├── main.cpp                    # 애플리케이션 진입점
├── server/
│   ├── Server.h/.cpp          # Builder 패턴의 메인 서버 클래스
│   ├── ServerConfig.h/.cpp    # 구성 관리
│   ├── reactor/               # 이벤트 루프 구현
│   │   ├── Reactor.h          # 추상 리액터 인터페이스
│   │   ├── ReactorTCP.cpp     # TCP 전용 리액터
│   │   └── ReactorUDP.cpp     # UDP 전용 리액터
│   ├── session/               # 연결 관리
│   │   ├── ClientSessionTCP.cpp
│   │   └── ClientSessionUDP.cpp
│   ├── threadpool/            # 워커 스레드 관리
│   ├── dispatcher/            # 메시지 라우팅
│   ├── filter/                # 요청 전처리
│   ├── handler/               # 이벤트 핸들러
│   └── messages/              # 메시지 파싱
└── utils/                     # 유틸리티 함수
```

---

## 테스트

프로젝트는 Google Test 프레임워크를 사용한 포괄적인 단위 테스트를 포함합니다:

```bash
make test
```

테스트 범위:
- 서버 초기화 및 구성
- 워커 큐 작업
- 클라이언트 세션 관리
- 필터 체인 처리

---

## 라이선스

이 프로젝트는 여러 라이선스 옵션으로 제공됩니다:

- **교육용**: 학습 및 교육 목적으로 무료
- **개인 및 소규모 프로젝트**: MIT 라이선스
- **기업 및 상업적 용도**: 별도의 라이선스 계약 필요

자세한 내용은 LICENSE 파일을 참조하세요.

---

## 기여하기

프레임워크 개선을 위한 기여를 환영합니다:

1. 저장소 포크
2. 기능 브랜치 생성
3. 적절한 테스트와 함께 변경사항 구현
4. 풀 리퀘스트 제출

버그 리포트 및 기능 요청은 [GitHub Issues](https://github.com/ImGdevel/Socket-Server-Framework/issues)를 사용해 주세요.