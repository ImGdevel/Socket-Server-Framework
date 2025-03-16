#include "../src/server/session/ClientSession.h"
#include <gtest/gtest.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

int mockSocket = 42;

// send 함수를 모킹하기 위한 헬퍼 함수
ssize_t mockSend(int sockfd, const void *buf, size_t len, int flags) {
    return len;
}

// ClientSession 테스트 클래스
class ClientSessionTest : public ::testing::Test {
protected:
    ClientSession* session;

    //ClientSession 객체 생성
    void SetUp() override {
        session = new ClientSession(mockSocket);
    }

    // 세션 객체 삭제
    void TearDown() override {
        delete session;
    }
};

// 생성자와 소멸자 테스트
TEST_F(ClientSessionTest, ConstructorAndDestructor) {
    EXPECT_EQ(session->getSocket(), mockSocket); 
    EXPECT_TRUE(session->isActive());  // 활성 상태
}

// appendToBuffer와 extractMessage 테스트
TEST_F(ClientSessionTest, AppendAndExtractMessage) {
    const std::string data = "Hello, World!";

    // 먼저 메시지 길이를 uint32_t 형식으로 전송할 크기 계산
    uint32_t messageLength = htonl(data.size()); 
    session->appendToBuffer(reinterpret_cast<const char*>(&messageLength), sizeof(messageLength)); 
    session->appendToBuffer(data.c_str(), data.size()); 

    std::string extractedMessage;
    
    EXPECT_TRUE(session->extractMessage(extractedMessage));  
    
    EXPECT_EQ(extractedMessage, data);  
}

// sendMessage 테스트
TEST_F(ClientSessionTest, SendMessage) {
    std::string message = "Test message";
    
    // 데이터 전송 길이가 올바른지 확인
    ssize_t result = mockSend(mockSocket, message.c_str(), message.size(), 0);
    EXPECT_EQ(result, message.size());  
}

// closeSession 테스트
TEST_F(ClientSessionTest, CloseSession) {
    session->closeSession();
    EXPECT_FALSE(session->isActive());
}

// setProcessing과 isProcessing 테스트
TEST_F(ClientSessionTest, SetAndCheckProcessing) {
    session->setProcessing(true);
    EXPECT_TRUE(session->isProcessing()); 
    
    session->setProcessing(false);
    EXPECT_FALSE(session->isProcessing()); 
}

// setCurrentRoom과 getCurrentRoom 테스트
TEST_F(ClientSessionTest, SetAndGetCurrentRoom) {
    session->setCurrentRoom("Room1");
    EXPECT_EQ(session->getCurrentRoom(), "Room1");
}
