#pragma once
#include <chrono>
#include <concepts>
#include <print>
#include <cstdint>
#include <string_view>

namespace timer {

using clock = std::chrono::high_resolution_clock;

double time(std::invocable auto init, std::invocable auto fn, std::uint32_t repeats) {
    auto total = 0.0;
    while (repeats--) {
        init();
        auto start = clock::now();
        fn();
        auto end = clock::now();
        total += std::chrono::duration<double>(end - start).count();
    }   
    return total; 
}

void time_print(std::invocable auto init, std::invocable auto fn, std::uint32_t repeats, std::string_view string) {
    auto elapsed = time(init, fn, repeats);
    std::print("Executing {} {} times took {:.6f} seconds.\n", string, repeats, elapsed);
}

void time_print(std::invocable auto fn, std::uint32_t repeats, std::string_view string) {
    auto elapsed = time([](){}, fn, repeats);
    std::print("Executing {} {} times took {:.6f} seconds.\n", string, repeats, elapsed);
}

} // namespace timer
