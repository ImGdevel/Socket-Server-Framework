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

# nlohmann/json 설정
NLOHMANN_JSON_DIR = $(EXTERNAL_DIR)/nlohmann_json
NLOHMANN_JSON_INCLUDE = $(NLOHMANN_JSON_DIR)/include

# 소스 및 테스트 파일 경로
SRC_DIR = src
APP_DIR = $(SRC_DIR)/server
UTILS_DIR = src/utils
TEST_DIR = tests/unit
Message_DIR = src/server/messages

# 인클루드 및 유틸리티 경로 설정
INCLUDE_DIRS = $(UTILS_DIR) $(GTEST_INCLUDE) $(RAPIDJSON_INCLUDE) $(Message_DIR)

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
	  $(APP_DIR)/handler/TestJSONEventHandler.cpp \
      $(APP_DIR)/chat/ChatRoom.cpp \
      $(APP_DIR)/chat/ChatRoomManager.cpp

# 테스트 파일
TEST_SRC = $(TEST_DIR)/ServerTest.cpp \
           $(TEST_DIR)/ChatRoomTest.cpp

# 오브젝트 파일 경로
OBJ_DIR = build/obj
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TEST_OBJ = $(TEST_SRC:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# 인클루드 경로
INCLUDES = $(addprefix -I, $(INCLUDE_DIRS))

# 기본 타겟: 서버 빌드 
all: check-dependencies $(TARGET)

# 외부 라이브러리 확인 및 다운로드
check-dependencies: $(PROTOBUF_INCLUDE) $(LIBXML2_INCLUDE) $(GTEST_LIB) $(RAPIDJSON_INCLUDE) $(NLOHMANN_JSON_INCLUDE)

# 테스트 실행
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# clean 규칙
clean:
	rm -f $(TEST_EXEC)
	rm -rf $(OBJ_DIR)

clean-external:
	rm -rf $(EXTERNAL_DIR)

clean-all:
	rm -f $(TEST_EXEC)
	rm -rf $(OBJ_DIR)
	rm -rf $(EXTERNAL_DIR)

# 다운로드 및 빌드 타겟
download: $(GTEST_LIB) $(RAPIDJSON_INCLUDE) $(NLOHMANN_JSON_INCLUDE)


########## [ 빌드 규칙 ] #############

# Google Test 설치 확인 및 빌드
$(GTEST_LIB):
	@if [ ! -d "$(GTEST_DIR)" ]; then \
		echo "Google Test not found. Downloading..."; \
		git clone --depth=1 https://github.com/google/googletest.git $(GTEST_DIR); \
		rm -rf $(GTEST_DIR)/.git; \
		cd $(GTEST_DIR) && cmake . && make; \
	fi

# Protobuf 설치 확인
$(PROTOBUF_INCLUDE):
	@if ! command -v protoc > /dev/null || [ ! -f /usr/lib/x86_64-linux-gnu/libprotobuf.so ]; then \
		echo "Protobuf compiler or library not found. Installing..."; \
		sudo apt-get update && sudo apt-get install -y protobuf-compiler libprotobuf-dev; \
	fi

# libxml2 설치 확인
$(LIBXML2_INCLUDE):
	@if ! ldconfig -p | grep -q "libxml2.so"; then \
		echo "libxml2 not found. Installing..."; \
		sudo apt-get update && sudo apt-get install -y libxml2 libxml2-dev; \
	fi

# RapidJSON 설치 확인
$(RAPIDJSON_INCLUDE):
	@if [ ! -d "$(RAPIDJSON_DIR)" ]; then \
		echo "RapidJSON not found. Downloading..."; \
		git clone --depth=1 https://github.com/Tencent/rapidjson.git $(RAPIDJSON_DIR); \
		rm -rf $(RAPIDJSON_DIR)/.git; \
	fi

# nlohmann/json 설치 확인
$(NLOHMANN_JSON_INCLUDE):
	@if [ ! -d "$(NLOHMANN_JSON_DIR)" ]; then \
		echo "nlohmann/json not found. Downloading..."; \
		git clone --depth=1 https://github.com/nlohmann/json.git $(NLOHMANN_JSON_DIR); \
		rm -rf $(NLOHMANN_JSON_DIR)/.git; \
	fi

# 서버 빌드 규칙
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) -lprotobuf -lxml2

# 테스트 실행 파일 빌드
$(TEST_EXEC): $(OBJ) $(TEST_OBJ)
	$(CXX) $(OBJ) $(TEST_OBJ) -o $(TEST_EXEC) -pthread -lprotobuf -lxml2 -lgtest

# 오브젝트 파일 생성 규칙
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
