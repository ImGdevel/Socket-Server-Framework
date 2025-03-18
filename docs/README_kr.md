[English](https://github.com/ImGdevel/Custom-Socket-Server) | 한국어


# Introduce

이 프로젝트는 기존 [프로토타입 서버](https://github.com/ImGdevel/MJU-backend2024/tree/main/chat_server) 을 개선하고 고성능 네트워크 서버를 구축하기 위해 시작되었습니다.
해당 프로젝트는  **epoll 기반 비동기 I/O + 스레드 풀 구조**를 적용하여 높은 동시성 처리 성능을 목표로 개발했습니다.

> ### 개발 배경
> 고성능 네트워크 서버 구축을 목표로 이 프로젝트를 시작했습니다.
> 초기에는 학사과정에 시작한 프로토타입 서버에서 출발했지만, 해당 서버는 단순한 구조였고, 실전 환경에서 사용하기엔 성능과 안정성이 부족했습니다.
> 이를 계기로 단순한 학습용 서버가 아닌 실제 서비스에 적용할 수 있는 범용 소켓 서버 프레임워크를 직접 개발하고 싶었습니다.
> 특히, 게임 서버 및 대규모 트래픽을 처리하는 시스템에서도 활용 가능하도록 확장성, 성능, 그리고 안정성을 최우선으로 고려하여 설계했습니다.

<br>

---

## 핵심 목표
- 고성능 & 확장성 – epoll 기반 비동기 I/O와 스레드 풀 구조를 활용하여 수만 개의 동시 접속을 안정적으로 처리
- 모듈화된 설계 – 데이터베이스 연동, 멀티 리액터 패턴, 다양한 API 및 메시지 포맷(JSON, Protobuf 등) 지원 가능
- 게임 서버 최적화 – TCP/UDP 동시 지원, 저지연 패킷 처리, 커스텀 프로토콜 확장 가능
- 실제 운영 환경 적용 가능 – 단순한 실습용이 아닌, 실제 서비스에 적용할 수 있는 구조

이 프로젝트는 단순한 소켓 서버가 아니라, 게임 및 다양한 실시간 서비스에서 활용할 수 있는 범용 네트워크 서버 프레임워크를 목표로 합니다.

<br>

## 프로젝트 구조

프로젝트 구조는 여기서 확인할 수 있습니다.


<br>

---

## 시작하기

### 1. 요구 사항
   
프로젝트를 빌드하고 실행하기 위해 다음의 패키지가 필요합니다.
- C++17 이상 지원 컴파일러 (예: g++)
- CMake (Google Test 빌드)
- Make (프로그램 빌드)

Linux 환경의 경우 설치는 다음과 같습니다.
```shell
sudo apt update
sudo apt install -y g++ cmake make
```

### 2. 프로젝트 클론 및 빌드

```shell
git clone https://github.com/your-repo/socket-server-framework.git
cd socket-server-framework
make
```

### 3. 실행

```shell
./build/server
```

<br>

## Docker로 실행

### 1. Docker 이미지 빌드
```shell
docker build -t socket-server .
```

### 2. 컨테이너 실행
```shell
docker run --rm -p 8080:8080 socket-server
```
  | 기본적으로 서버는 8080 포트를 사용합니다. 필요에 따라 -p <호스트포트>:<컨테이너포트>로 변경 가능합니다.

<br>

## 추가 명령어

```
make download // 의존성 외부 라이브러리 설치(local)
make test  // 테스트 실행
make clean // 빌드 결과 삭제
make clean-all // 빌드 및 의존성 외부 라이브러리 전체 삭제
```
