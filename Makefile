
CXX = g++
CXXFLAGS = -std=c++17 -Wall

TARGET = build/server

SRC = main.cpp src/Server.cpp src/reactor/Reactor.cpp src/reactor/Worker.cpp src/session/ClientSession.cpp src/threadpool/ThreadPool.cpp
OBJ = $(SRC:.cpp=.o)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)


%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
