CXX := c++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude
TARGET := expense_tracker
SOURCES := $(wildcard src/*.cpp)
HEADERS := $(wildcard include/*.h)

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

test: $(TARGET)
	sh tests/run_tests.sh

clean:
	rm -f $(TARGET)
