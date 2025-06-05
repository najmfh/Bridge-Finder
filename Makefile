# Nama file output
TARGET = BridgeFinder

# Compiler yang digunakan
CXX = g++
CXXFLAGS = -Wall -std=c++17 -g

# File sumber .cpp
SRCS = main.cpp graph.cpp bridgeAlgorithm.cpp

# File object .o (otomatis dari SRCS)
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp graph.h bridgeAlgorithm.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

graph.o: graph.cpp graph.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

bridgeAlgorithm.o: bridgeAlgorithm.cpp bridgeAlgorithm.h graph.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run