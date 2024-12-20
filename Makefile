
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -Isrc/utils

TARGET = build/server

SRC = src/main.cpp src/server/Server.cpp src/server/reactor/Reactor.cpp src/server/reactor/Worker.cpp src/server/session/ClientSession.cpp src/server/threadpool/ThreadPool.cpp src/server/dispatcher/MessageDispatcher.cpp src/server/dispatcher/HandlerConfigurator.cpp src/server/handler/TestEventHandler.cpp src/server/chat/ChatRoom.cpp src/server/chat/ChatRoomManager.cpp
OBJ = $(SRC:.cpp=.o)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	rm -f $(TARGET)
