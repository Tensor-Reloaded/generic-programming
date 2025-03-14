#include "parallel_algorithms.h"
#include "timer.h"
#include "random_init.h"

#include <cstdint>
#include <execution>
#include <algorithm>


void do_not_optimize(auto x) {
    [[maybe_unused]] volatile auto y = x;
}

void test_find() {
    constexpr auto size = 700'000'000;
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
    auto stl_find_seq = [last, &vec]() {
        do_not_optimize(std::find_if(std::execution::seq, vec.begin(), vec.end(), [last](auto elem) {
            return elem == last;
        }));
    };
    auto stl_find_par = [last, &vec]() {
        do_not_optimize(std::find_if(std::execution::par, vec.begin(), vec.end(), [last](auto elem) {
            return elem == last;
        }));
    };
    auto stl_find_par_unseq = [last, &vec]() {
        do_not_optimize(std::find_if(std::execution::par_unseq, vec.begin(), vec.end(), [last](auto elem) {
            return elem == last;
        }));
    };
    auto stl_find_unseq = [last, &vec]() {
        do_not_optimize(std::find_if(std::execution::unseq, vec.begin(), vec.end(), [last](auto elem) {
            return elem == last;
        }));
    };
    timer::time_print(find_seq, 5, "Sequential find");
    timer::time_print(find_par, 5, "Parallel find");
    timer::time_print(stl_find_seq, 5, "STL sequential find");
    timer::time_print(stl_find_par, 5, "STL parallel find");
    timer::time_print(stl_find_par_unseq, 5, "STL parallel unseq find");
    timer::time_print(stl_find_unseq, 5, "STL unseq find");
}


int main() {
    test_find();
}