# 컴파일러 설정
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

# 타겟 설정
TARGET = build/server
TEST_EXEC = build/test

# 외부 라이브러리 설정
EXTERNAL_DIR = external

### Google Test 설정
GTEST_DIR = $(EXTERNAL_DIR)/googletest
GTEST_LIB = $(GTEST_DIR)/build/lib/libgtest.a
GTEST_INCLUDE = $(GTEST_DIR)/googletest/include

### RapidJSON 설정
RAPIDJSON_DIR = $(EXTERNAL_DIR)/rapidjson
RAPIDJSON_INCLUDE = $(RAPIDJSON_DIR)/include

# 소스 및 테스트 파일 경로
SRC_DIR = src
APP_DIR = $(SRC_DIR)/server
UTILS_DIR = src/utils
TEST_DIR = tests/unit

# 인클루드 및 유틸리티 경로 설정
INCLUDE_DIRS = $(UTILS_DIR) $(GTEST_INCLUDE) $(RAPIDJSON_INCLUDE)

# 소스 파일
SRC = $(SRC_DIR)/main.cpp \
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
OBJ_DIR = build/obj
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TEST_OBJ = $(TEST_SRC:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# 인클루드 경로 설정 (동적으로 추가)
INCLUDES = $(addprefix -I, $(INCLUDE_DIRS))

# 기본 타겟: 서버 빌드
all: $(TARGET)

# 외부 라이브러리 확인 및 다운로드
$(GTEST_LIB):
	@if [ ! -d "$(GTEST_DIR)" ]; then \
		echo "Google Test not found. Downloading..."; \
		git clone --depth=1 https://github.com/google/googletest.git $(GTEST_DIR); \
		rm -rf $(GTEST_DIR)/.git; \
		cmake -S $(GTEST_DIR) -B $(GTEST_DIR)/build; \
		cmake --build $(GTEST_DIR)/build; \
	fi

$(RAPIDJSON_INCLUDE):
	@if [ ! -d "$(RAPIDJSON_DIR)" ]; then \
		echo "RapidJSON not found. Downloading..."; \
		git clone --depth=1 https://github.com/Tencent/rapidjson.git $(RAPIDJSON_DIR); \
		rm -rf $(RAPIDJSON_DIR)/.git; \
	fi

# 서버 빌드 규칙
$(TARGET): $(OBJ) $(GTEST_LIB) $(RAPIDJSON_INCLUDE)
	$(CXX) $(OBJ) -o $(TARGET)

# 테스트 실행 파일 빌드
$(TEST_EXEC): $(OBJ) $(TEST_OBJ) $(GTEST_LIB) $(RAPIDJSON_INCLUDE)
	$(CXX) $(OBJ) $(TEST_OBJ) $(GTEST_LIB) -o $(TEST_EXEC) -pthread

# 오브젝트 파일 생성 규칙
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# 테스트 실행
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# clean 규칙
clean:
	rm -f $(TEST_EXEC)
	rm -rf $(OBJ_DIR)

clean_external:
	rm -rf $(EXTERNAL_DIR)

clean_all:
	rm -f $(TEST_EXEC)
	rm -rf $(OBJ_DIR)
	rm -rf $(EXTERNAL_DIR)

# 다운로드 및 빌드 타겟
download: $(GTEST_LIB) $(RAPIDJSON_INCLUDE)
