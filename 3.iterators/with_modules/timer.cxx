module;
#include <chrono>
#include <concepts>
#include <print>
#include <ranges>
#include <cmath>
#include <string_view>

export module timer;
import algorithms;


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

export void repeat(std::invocable auto fn, std::uint32_t repeats, std::uint32_t tests, std::string_view s) {
    std::vector<double> times;
    times.reserve(tests);

    while (tests--) {
        times.push_back(time(fn, repeats));
    }

    double mean = reduce(times.begin(), times.end()) / times.size();
    double variance_sum = reduce(times.begin(), times.end(), 0.0, [mean](double init, double current) {
        return init + current - mean;
    });

    std::print("Mean is {:.6f} sec, stddev is {:.6f} sec, {}\n", mean, std::sqrt(variance_sum / times.size()), s);
}

} // namespace timer
