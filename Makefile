# 컴파일러 설정
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -Isrc/utils -Iexternal/googletest/googletest/include

# 타겟 설정
TARGET = build/server
TEST_EXEC = build/test

# Google Test 설정
GTEST_DIR = external/googletest
GTEST_LIB = $(GTEST_DIR)/build/lib/libgtest.a
GTEST_INCLUDE = $(GTEST_DIR)/googletest/include

# 소스 및 테스트 파일 경로
SRC_DIR = src
APP_DIR = $(SRC_DIR)/server
TEST_DIR = tests/unit

# 소스 파일
SRC = src/main.cpp \
      $(APP_DIR)/Server.cpp \
      $(APP_DIR)/reactor/Reactor.cpp \
      $(APP_DIR)/reactor/Worker.cpp \
      $(APP_DIR)/session/ClientSession.cpp \
      $(APP_DIR)/threadpool/ThreadPool.cpp \
      $(APP_DIR)/dispatcher/MessageDispatcher.cpp \
      $(APP_DIR)/dispatcher/HandlerConfigurator.cpp \
      $(APP_DIR)/handler/TestEventHandler.cpp \
      $(APP_DIR)/chat/ChatRoom.cpp \
      $(APP_DIR)/chat/ChatRoomManager.cpp

# 테스트 파일
TEST_SRC = $(TEST_DIR)/ServerTest.cpp \
           $(TEST_DIR)/ChatRoomTest.cpp

# 오브젝트 파일 경로
OBJ = $(SRC:.cpp=.o)
TEST_OBJ = $(TEST_SRC:.cpp=.o)



# 기본 타겟: 서버 빌드
all: $(TARGET)

# 서버 빌드 규칙
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

# 테스트 실행 파일 빌드
$(TEST_EXEC): $(OBJ) $(TEST_OBJ) $(GTEST_LIB)
	$(CXX) $(OBJ) $(TEST_OBJ) $(GTEST_LIB) -o $(TEST_EXEC) -pthread

# 오브젝트 파일 생성 규칙
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 테스트 실행
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# clean 규칙
clean:
	rm -f $(OBJ)
	rm -f $(TARGET)
