COMPILER = g++-14
BUILD_DIR = build
OPTIMIZATION = -O2
COMPILER_FLAG = -fmodules-ts -Wall -Wextra ${OPTIMIZATION} -std=c++23
TIME_CMD = /usr/bin/time -f "\nElapsed Time: %E User Time: %U System Time: %S CPU Usage: %P"
EXE ?= main

default: all

time:
	${TIME_CMD} ./$(BUILD_DIR)/${EXE}.exe

all: compile run

run:
	./$(BUILD_DIR)/main.exe


compile: $(BUILD_DIR)/my_module.o $(BUILD_DIR)/main.o
	${COMPILER} ${COMPILER_FLAG} $^ -o $(BUILD_DIR)/main.exe

$(BUILD_DIR)/%.o: %.cxx  | $(BUILD_DIR)
	${COMPILER} -c ${COMPILER_FLAG} $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf gcm.cache
