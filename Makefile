CXX := g++
CXXFLAGS := -std=gnu++20 -Wall -Wextra -g -Iheaders

SRC_DIR := src
SRCS := $(SRC_DIR)/gameclub.cpp $(SRC_DIR)/format.cpp $(SRC_DIR)/modules.cpp
MAIN_SRC := main.cpp

OBJS := $(SRCS:.cpp=.o) $(MAIN_SRC:.cpp=.o)

TARGET := GameClub

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp headers/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: main.cpp headers/gameclub.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

test: $(TARGET)
	echo "test#1"
	./$(TARGET) file_test.txt
	echo "test#2"
	./$(TARGET) test_queue.txt
	echo "test#3"
	./$(TARGET) error_format.txt

.PHONY: all clean test