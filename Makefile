CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -g

SRC_DIR = src
BUILD_DIR = build/Debug
TARGET = $(BUILD_DIR)/outDebug

SRCS = $(shell find $(SRC_DIR) -name "*.cpp")

all: build

build:
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)
	@echo "$(TARGET)"

run: build
	./$(TARGET)

debug: build
	gdb ./$(TARGET)

clean:
	rm -rf build
