COMPILER = g++-14
BUILD_DIR = build
OPTIMIZATION = -O2
COMPILER_FLAG = -fmodules-ts -Wall -Wextra ${OPTIMIZATION} -std=c++23
TIME_CMD = /usr/bin/time -f "\nElapsed Time: %E User Time: %U System Time: %S CPU Usage: %P"
EXE ?= main

default: all

time:
	${TIME_CMD} ./$(BUILD_DIR)/${EXE}.exe

all: compile_canvas compile_addable

canvas: compile_canvas run_canvas

addable: compile_addable run_addable

run_canvas:
	./$(BUILD_DIR)/canvas.exe

run_addable:
	./$(BUILD_DIR)/addable.exe

compile_canvas: $(BUILD_DIR)
	${COMPILER} ${COMPILER_FLAG} canvas.cxx -o $(BUILD_DIR)/canvas.exe

compile_addable: $(BUILD_DIR)
	${COMPILER} ${COMPILER_FLAG} addable.cxx -o $(BUILD_DIR)/addable.exe

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf gcm.cache
