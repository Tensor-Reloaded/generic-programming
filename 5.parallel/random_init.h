#include <algorithm>
#include <random>
#include <vector>


template <class T, class Dtype = typename T::value_type>
T random_container(int size) {
    auto rand = std::default_random_engine(3);
    auto v = std::vector<Dtype>(size);
    std::iota(v.begin(), v.end(), Dtype{0});
    std::ranges::shuffle(v, rand);
    return T(v.begin(), v.end());
}

template <class T, class Dtype = typename T::value_type>
T random_container_floating(int size) {
    auto rand = std::default_random_engine(3);
    std::uniform_real_distribution<Dtype> dist(static_cast<Dtype>(0.0), static_cast<Dtype>(1.0));

    std::vector<Dtype> v(size);
    for (auto &val : v) {
        val = dist(rand);
    }
    return T(v.begin(), v.end());
}
