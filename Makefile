
CXX = g++
CXXFLAGS = -std=c++11 -Wall

TARGET = server

SRC = main.cpp src/Server.cpp src/reactor/Reactor.cpp
OBJ = $(SRC:.cpp=.o)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)


%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
