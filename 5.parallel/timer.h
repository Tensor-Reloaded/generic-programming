#include <chrono>
#include <concepts>
#include <print>
#include <cstdint>
#include <string_view>

namespace timer {

using clock = std::chrono::high_resolution_clock;

double time(std::invocable auto fn, std::uint32_t repeats) {
    auto start = clock::now();
    while (repeats--) {
        fn();
    }
    auto end = clock::now();
    return std::chrono::duration<double>(end - start).count();
}

void time_print(std::invocable auto fn, std::uint32_t repeats, std::string_view string) {
    auto elapsed = time(fn, repeats);
    std::print("Executing {} {} times took {:.6f} seconds.\n", string, repeats, elapsed);
}

} // namespace timer
