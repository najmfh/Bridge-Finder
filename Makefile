# Nama file output
TARGET = BridgeFinder

# Compiler yang digunakan
CXX = g++
CXXFLAGS = -Wall -std=c++17

# File sumber
SRCS = main.cpp graph.cpp bridgeAlgorithm.cpp

# File object
OBJS = $(SRCS:.cpp=.o)

# Rule utama untuk build
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule untuk bersih-bersih
clean:
	rm -f $(OBJS) $(TARGET)