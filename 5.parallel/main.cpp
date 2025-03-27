#include "parallel_algorithms.h"
#include "timer.h"
#include "random_init.h"

#include <cstdint>
#include <execution>
#include <algorithm>


void do_not_optimize(auto x) {
    [[maybe_unused]] volatile auto y = x;
}

void test_sort() {
    constexpr auto size = 10'000'000;
    auto vec = random_container<std::vector<std::int32_t>>(size);
    auto vec_copy = vec;

    auto init = [&vec, &vec_copy]() {
        vec_copy  = vec; // we reinitialize
    };
    auto sort_seq = [&vec_copy]() {
        std::sort(std::execution::seq, vec_copy.begin(), vec_copy.end());
        do_not_optimize(vec_copy.begin());
    };
    auto sort_par = [&vec_copy]() {
        std::sort(std::execution::par, vec_copy.begin(), vec_copy.end());
        do_not_optimize(vec_copy.begin());
    };
    auto sort_par_unseq = [&vec_copy]() {
        std::sort(std::execution::par_unseq, vec_copy.begin(), vec_copy.end());
        do_not_optimize(vec_copy.begin());
    };
    auto sort_unseq = [&vec_copy]() {
        std::sort(std::execution::unseq, vec_copy.begin(), vec_copy.end());
        do_not_optimize(vec_copy.begin());
    };
    
    timer::time_print(init, sort_seq, 5, "sequential sort");
    timer::time_print(init, sort_par, 5, "parallel sort");
    timer::time_print(init, sort_par_unseq, 5, "parallel unseq sort");
    timer::time_print(init, sort_unseq, 5, "unseq sort");
}


void test_find() {
    constexpr auto size = 500'000'000;
    auto vec = random_container<std::vector<std::int32_t>>(size);
    auto last = vec.back();

    auto find_seq = [last, &vec]() {
        do_not_optimize(our_find_if(vec.begin(), vec.end(), [last](auto elem) {
            return elem == last;
        }));
    };
    auto find_par = [last, &vec]() {
        do_not_optimize(parallel_find_if(vec.begin(), vec.end(), [last](auto elem) {
            return elem == last;
        }));
    };
    timer::time_print(find_seq, 10, "Sequential find");
    timer::time_print(find_par, 10, "Parallel find");
}


int main() {
    test_find();
    test_sort();
}