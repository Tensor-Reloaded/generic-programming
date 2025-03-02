# C++ Modules

See https://learn.microsoft.com/en-us/cpp/cpp/modules-cpp?view=msvc-170

***
## How to run
1. Use `g++`
```
mkdir -p build
g++-14 -c -fmodules-ts -Wall -Wextra -O2 -std=c++23 my_module.cxx -o build/my_module.o
g++-14 -c -fmodules-ts -Wall -Wextra -O2 -std=c++23 main.cxx -o build/main.o
g++-14 -fmodules-ts -Wall -Wextra -O2 -std=c++23 build/my_module.o build/main.o -o build/main.exe
./build/main.exe
```
* `-fmodules-ts` is a compiler flag that enables support for C++20 modules.
* `-Wall` and `Wextra` are compiler flags that enable additional warnings to help catch potential issues in the code.
* `-O2` is an optimization flag that enables a good balance of performance improvements. You may use `-O0` (no optimization), `-O1` (basic optimization), or `-O3` (aggressive optimization) instead.
* `-std=c++23` specifies the C++23 standard for compiling the code.
* `-c` tells the compiler to generate object files (.o) without linking.
* `-o` specifies the output file name for the compiled object or executable.

2. Use `make`
```
make
# Alternatively, you can use `make compile` and `make run`
make clean
# This cleans the module cache and the build directory
```
* Check the [Makefile](./Makefile)

## References
* https://en.cppreference.com/w/cpp/language/modules
