
CXX = g++
CXXFLAGS = -std=c++11 -Wall

TARGET = server

SRC = main.cpp src/Server.cpp src/Worker.cpp src/WorkerQueue.cpp src/ThreadPool.cpp
OBJ = $(SRC:.cpp=.o)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)


%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
