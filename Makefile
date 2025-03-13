# 컴파일러 기본 옵션
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# 빌드 결과 타겟 설정
TARGET = build/server
TEST_EXEC = build/test_runner

# 외부 라이브러리 설정
EXTERNAL_DIR = external

### Google Test 설정
GTEST_DIR = $(EXTERNAL_DIR)/googletest
GTEST_BUILD_DIR = $(GTEST_DIR)/build
GTEST_LIBS = $(GTEST_BUILD_DIR)/lib/libgtest.a $(GTEST_BUILD_DIR)/lib/libgtest_main.a
GTEST_INCLUDE = $(GTEST_DIR)/googletest/include $(GTEST_DIR)/googlemock/include

### RapidJSON 설정
RAPIDJSON_DIR = $(EXTERNAL_DIR)/rapidjson
RAPIDJSON_INCLUDE = $(RAPIDJSON_DIR)/include

### nlohmann/json 설정
NLOHMANN_JSON_DIR = $(EXTERNAL_DIR)/nlohmann_json
NLOHMANN_JSON_INCLUDE = $(NLOHMANN_JSON_DIR)/include

### MySQL 설정
MYSQL_LIB = /usr/lib/x86_64-linux-gnu/libmysqlclient.so
MYSQL_INCLUDE = /usr/include/mysql

### Redis (hiredis) 설정
HIREDIS_DIR = $(EXTERNAL_DIR)/hiredis
HIREDIS_LIB = $(HIREDIS_DIR)/libhiredis.a
HIREDIS_INCLUDE = $(HIREDIS_DIR)/hiredis

# 소스 및 테스트 파일 경로
SRC_DIR = src
APP_DIR = $(SRC_DIR)/server
UTILS_DIR = src/utils
TEST_DIR = test
UNIT_TEST_DIR = $(TEST_DIR)/unit
INTEGRATION_TEST_DIR = $(TEST_DIR)/integration
Message_DIR = src/server/messages


# 소스 파일
SRC = $(SRC_DIR)/main.cpp \
      $(APP_DIR)/Server.cpp \
      $(APP_DIR)/reactor/Reactor.cpp \
      $(APP_DIR)/reactor/Worker.cpp \
      $(APP_DIR)/session/ClientSession.cpp \
      $(APP_DIR)/threadpool/ThreadPool.cpp \
      $(APP_DIR)/dispatcher/MessageDispatcher.cpp \
      $(APP_DIR)/dispatcher/HandlerConfigurator.cpp \
	  $(APP_DIR)/dispatcher/MessageDispatcherFactory.cpp \
      $(APP_DIR)/handler/TestEventHandler.cpp \
	  $(APP_DIR)/handler/TestJSONEventHandler.cpp \
      $(APP_DIR)/chat/ChatRoom.cpp \
      $(APP_DIR)/chat/ChatRoomManager.cpp

# 테스트 파일
TEST_SRC = 	$(TEST_DIR)/main_test.cpp \
			$(UNIT_TEST_DIR)/WorkerQueueTest.cpp \
			$(UNIT_TEST_DIR)/WorkerTest.cpp


# 오브젝트 파일 경로
OBJ_DIR = build/obj
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TEST_OBJ = $(TEST_SRC:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# 인클루드 경로
INCLUDE_DIRS = $(SRC_DIR) $(GTEST_INCLUDE) $(RAPIDJSON_INCLUDE) $(NLOHMANN_JSON_DIR)/include \
               $(HIREDIS_INCLUDE) $(MYSQL_INCLUDE) $(UTILS_DIR) $(Message_DIR)
INCLUDES = $(addprefix -I, $(INCLUDE_DIRS))

# 기본 타겟: 서버 빌드 
all: check-dependencies $(TARGET)

# 외부 라이브러리 확인 및 다운로드
check-dependencies: $(PROTOBUF_INCLUDE) $(LIBXML2_INCLUDE) $(GTEST_LIBS) $(RAPIDJSON_INCLUDE) $(NLOHMANN_JSON_INCLUDE) $(MYSQL_LIB) $(HIREDIS_LIB)

# 테스트 실행
test: check-dependencies $(TEST_EXEC)
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
download: $(GTEST_LIBS) $(RAPIDJSON_INCLUDE) $(NLOHMANN_JSON_INCLUDE) $(MYSQL_LIB) $(HIREDIS_LIB)

########## [ 빌드 규칙 ] #############

# Google Test 빌드 규칙
$(GTEST_LIBS):
	@if [ ! -d "$(GTEST_DIR)" ]; then \
		echo "Google Test not found. Downloading..."; \
		git clone --depth=1 https://github.com/google/googletest.git $(GTEST_DIR); \
		rm -rf $(GTEST_DIR)/.git; \
	fi
	@if [ ! -d "$(GTEST_BUILD_DIR)" ]; then \
		mkdir -p $(GTEST_BUILD_DIR); \
		cd $(GTEST_BUILD_DIR) && cmake .. && make; \
	fi
	@ls -l $(GTEST_BUILD_DIR)/lib/

# MySQL 설치 확인
$(MYSQL_LIB):
	@if ! dpkg -s libmysqlclient-dev > /dev/null 2>&1; then \
		echo "MySQL client library not found. Installing..."; \
		sudo apt-get update && sudo apt-get install -y libmysqlclient-dev; \
	fi

# Redis (hiredis) 설치 확인 및 빌드
$(HIREDIS_LIB):
	@if [ ! -d "$(HIREDIS_DIR)" ]; then \
		echo "hiredis not found. Downloading..."; \
		git clone --depth=1 https://github.com/redis/hiredis.git $(HIREDIS_DIR); \
		rm -rf $(HIREDIS_DIR)/.git; \
		cd $(HIREDIS_DIR) && make && sudo make install; \
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
	$(CXX) $(OBJ) -o $(TARGET) -lprotobuf -lxml2 -lmysqlclient -lhiredis

# 테스트 실행 파일 빌드
$(TEST_EXEC): $(TEST_OBJ) $(OBJ) $(GTEST_LIBS)
	$(CXX) $(TEST_OBJ) $(filter-out build/obj/main.o, $(OBJ)) $(GTEST_LIBS) -o $(TEST_EXEC) -pthread

# 오브젝트 파일 생성 규칙
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
