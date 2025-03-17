COMPILER = g++-14
BUILD_DIR = build
OPTIMIZATION ?= -O2
COMPILER_FLAG = -Wall -Wextra ${OPTIMIZATION} -std=c++23
TIME_CMD = /usr/bin/time -f "\nElapsed Time: %E User Time: %U System Time: %S CPU Usage: %P"
EXE ?= main

default: all

time:
	${TIME_CMD} ./$(BUILD_DIR)/${EXE}.exe

all: clean compile run

run:
	./$(BUILD_DIR)/main.exe


compile: $(BUILD_DIR)
	${COMPILER} ${COMPILER_FLAG} -o $(BUILD_DIR)/main.exe main.cpp -ltbb

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	@rm -rf $(BUILD_DIR)
