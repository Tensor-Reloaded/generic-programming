#include <algorithm>
#include <deque>
#include <list>
#include <random>
#include <ranges>
#include <stdfloat>
#include <vector>

import algorithms;
import timer;

template <class T, class Dtype = typename T::value_type>
T random_container(int size) {
    auto rand = std::default_random_engine(3);
    auto v = std::vector<Dtype>(size);
    std::iota(v.begin(), v.end(), Dtype{});
    std::ranges::shuffle(v, rand);
    return T(v.begin(), v.end());
}

template <class T, class Dtype = typename T::value_type>
T random_container_floating(int size) {
    auto rand = std::default_random_engine(3);
    std::uniform_real_distribution<typename T::value_type> dist(static_cast<Dtype>(0.0), static_cast<Dtype>(1.0));

    std::vector<Dtype> v(size);

    for (auto &val : v) {
        val = dist(rand);
    }

    return T(v.begin(), v.end());
}

template <class T>
void test_find_for_container(std::string test_name) {
    constexpr auto size = 500'000'000;
    auto container = random_container<T>(size);
    container.back() = size;

    auto ranges_find = [&container, size]() {
        [[maybe_unused]] volatile auto it = std::ranges::find(container, size);
    };
    auto ranges_find_unreachable = [&container, size]() {
        [[maybe_unused]] volatile auto it = std::ranges::find(container.begin(), std::unreachable_sentinel, size);
    };
    auto stl_find = [&container, size]() {
        [[maybe_unused]] volatile auto it = std::find(container.begin(), container.end(), size);
    };
    auto our_find = [&container, size]() {
        [[maybe_unused]] volatile auto it = find(container, size);
    };
    auto our_find_unreachable = [&container, size]() {
        [[maybe_unused]] volatile auto it = find(container.begin(), std::unreachable_sentinel, size);
    };
    timer::repeat(ranges_find, 3, 3, test_name + " ranges find");
    timer::repeat(ranges_find_unreachable, 3, 3, test_name + " ranges find + unreachable");
    timer::repeat(stl_find, 3, 3, test_name + " stl find");
    timer::repeat(our_find, 3, 3, test_name + " our find");
    timer::repeat(our_find_unreachable, 3, 3, test_name + " our find + unreachable");
}

template <class T>
void test_inner_product(std::string test_name) {
    constexpr auto size = 500'000'000;
    auto container1 = random_container_floating<T>(size);
    auto container2 = random_container_floating<T>(size);

    auto stl_inner_product = [&container1, &container2]() {
        [[maybe_unused]] volatile auto result = std::inner_product(container1.begin(), container1.end(), container2.begin(), 0.0);
    };
    auto our_inner_product = [&container1, &container2]() {
        [[maybe_unused]] volatile auto result = inner_product(container1.begin(), container1.end(), container2.begin());
    };
    auto our_fast_inner_product = [&container1, &container2]() {
        [[maybe_unused]] volatile auto result = fast_inner_product(container1.begin(), container1.end(), container2.begin());
    };
    timer::repeat(stl_inner_product, 3, 3, test_name + " stl inner product");
    timer::repeat(our_inner_product, 3, 3, test_name + " our inner product");
    timer::repeat(our_fast_inner_product, 3, 3, test_name + " fast inner product");
}

int main() {
    test_find_for_container<std::vector<std::int32_t>>("vector i32");
    test_find_for_container<std::list<std::int32_t>>("list i32");

    test_find_for_container<std::vector<std::int64_t>>("vector i64");
    test_find_for_container<std::list<std::int64_t>>("list i64");

    test_inner_product<std::vector<std::float16_t>>("vector f16");
    test_inner_product<std::list<std::float16_t>>("list f16");

    test_inner_product<std::vector<std::float32_t>>("vector f32");
    test_inner_product<std::list<std::float32_t>>("list f32");

    test_inner_product<std::vector<std::float64_t>>("vector f64");
    test_inner_product<std::list<std::float64_t>>("list f64");
    return 0;
}
