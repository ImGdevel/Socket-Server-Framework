#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Server.h"
#include "IEventHandler.h"
#include "MessageDispatcher.h"
#include "ThreadPool.h"
#include "Reactor.h"
#include "ClientRequest.h"

using namespace std;
using namespace testing;

// Mock 클래스들 정의
class MockEventHandler : public IEventHandler {
public:
    MOCK_CONST_METHOD0(createHandlers, 
        std::unordered_map<std::string, std::function<void(const ClientRequest&)>>());
};

class MockThreadPool : public ThreadPool {
public:
    MockThreadPool(int numThreads) : ThreadPool(numThreads) {}
    MOCK_METHOD1(enqueueTask, void(const std::function<void()>&));
    MOCK_METHOD0(stop, void());
};

class MockMessageDispatcher : public MessageDispatcher {
public:
    explicit MockMessageDispatcher(std::unique_ptr<EventRegistry> registry) 
        : MessageDispatcher(std::move(registry)) {}
    MOCK_METHOD1(dispatch, void(const ClientRequest&));
};

class MockReactor : public Reactor {
public:
    MockReactor(int port, ThreadPool& tp, MessageDispatcher& md) 
        : Reactor(port, tp, md) {}
    MOCK_METHOD0(start, void());
    MOCK_METHOD0(stop, void());
};

class ServerTest : public Test {
protected:
    void SetUp() override {
        mockEventHandler = make_unique<MockEventHandler>();
        eventRegistry = make_unique<EventRegistry>();
        
        // 기본 반환값 설정
        EXPECT_CALL(*mockEventHandler, createHandlers())
            .WillRepeatedly(Return(std::unordered_map<std::string, std::function<void(const ClientRequest&)>>()));
    }

    unique_ptr<MockEventHandler> mockEventHandler;
    unique_ptr<EventRegistry> eventRegistry;
};

TEST_F(ServerTest, BuilderValidatesPortCorrectly) {
    Server::Builder builder;
    
    // 유효하지 않은 포트로 테스트
    EXPECT_THROW(builder.setPort(-1), invalid_argument);
    EXPECT_THROW(builder.setPort(0), invalid_argument);
    
    // 유효한 포트로 테스트
    EXPECT_NO_THROW(builder.setPort(8080));
}

TEST_F(ServerTest, BuilderValidatesWorkerCountCorrectly) {
    Server::Builder builder;
    
    // 유효하지 않은 워커 수로 테스트
    EXPECT_THROW(builder.setWorkerCount(-1), invalid_argument);
    EXPECT_THROW(builder.setWorkerCount(0), invalid_argument);
    
    // 유효한 워커 수로 테스트
    EXPECT_NO_THROW(builder.setWorkerCount(4));
}

TEST_F(ServerTest, BuilderValidatesMessageTypeCorrectly) {

}

TEST_F(ServerTest, BuilderCreatesServerSuccessfully) {
    // Mock 핸들러 설정
    EXPECT_CALL(*mockEventHandler, createHandlers())
        .WillRepeatedly(Return(std::unordered_map<std::string, std::function<void(const ClientRequest&)>>()));

    Server::Builder builder;
    
    // 서버 생성 테스트
    EXPECT_NO_THROW({
        auto server = builder
            .setPort(8080)
            .setWorkerCount(4)
            .setMessageType("JSON")
            .setEventHandler(*mockEventHandler)
            .build();
    });
}

TEST_F(ServerTest, BuildFailsWithoutRequiredParameters) {
    Server::Builder builder;
    
    // 필수 파라미터 없이 빌드 시도
    EXPECT_THROW(builder.build(), invalid_argument);
    
    // 일부 파라미터만 설정
    builder.setPort(8080);
    EXPECT_THROW(builder.build(), invalid_argument);
    
    builder.setWorkerCount(4);
    EXPECT_THROW(builder.build(), invalid_argument);
}

TEST_F(ServerTest, ServerLifecycleTest) {
    // Mock 핸들러 설정
    EXPECT_CALL(*mockEventHandler, createHandlers())
        .WillRepeatedly(Return(std::unordered_map<std::string, std::function<void(const ClientRequest&)>>()));

    Server::Builder builder;
    auto server = builder
        .setPort(8080)
        .setWorkerCount(4)
        .setMessageType("JSON")
        .setEventHandler(*mockEventHandler)
        .build();

    // run과 terminate가 예외를 발생시키지 않는지 확인
    EXPECT_NO_THROW({
        server->run();
        server->terminate();
    });
}

// 추가: 서버 종료 시 리소스 정리 테스트
TEST_F(ServerTest, ServerCleanupTest) {
    // Mock 핸들러 설정
    EXPECT_CALL(*mockEventHandler, createHandlers())
        .WillRepeatedly(Return(std::unordered_map<std::string, std::function<void(const ClientRequest&)>>()));

    std::unique_ptr<Server> server;
    
    EXPECT_NO_THROW({
        Server::Builder builder;
        server = builder
            .setPort(8080)
            .setWorkerCount(4)
            .setMessageType("JSON")
            .setEventHandler(*mockEventHandler)
            .build();
    });

    // 서버 객체가 소멸될 때 예외가 발생하지 않아야 함
    EXPECT_NO_THROW({
        server.reset();
    });
}

