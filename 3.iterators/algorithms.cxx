module;
#include <concepts>
#include <ranges>

export module algorithms;

template <class T>
concept reducible = requires(T x, T y) {
    requires std::is_default_constructible_v<T> && std::copyable<T> && std::movable<T>;
    x += y;
    x = x + y;
    x = std::move(y);
};

template <class Fn, class T>
concept unary_function = std::invocable<Fn, T>;

template <class Fn, class T, class... Args>
concept returns_t = requires(Fn fn, Args &&...args) {
    requires std::invocable<Fn, Args...>;
    { fn(std::forward<Args>(args)...) } -> std::same_as<T>;
};

export template <std::input_iterator I, std::sentinel_for<I> S, class ReduceOp, class T = std::iter_value_t<I>>
    requires returns_t<ReduceOp, T, T, T> && std::copyable<T>
T reduce(I first, S last, T init, ReduceOp fn) {
    while (first != last) {
        init = fn(init, *first);
        ++first;
    }
    return init;
}

export template <std::input_iterator I, std::sentinel_for<I> S, class T = std::iter_value_t<I>>
    requires reducible<T>
T reduce(I first, S last) {
    T init{};
    while (first != last) {
        init += *first;
        ++first;
    }
    return init;
    // Or we can just call
    return reduce(first, last, T{}, std::plus<>());
}

export template <std::input_iterator I, std::sentinel_for<I> S, class Proj, class T = std::iter_value_t<I>>
    requires unary_function<Proj, T> && returns_t<Proj, T>
void transform(I first, S last, Proj proj) {
    while (first != last) {
        *first = proj(*first);
        ++first;
    }
}

export template <std::input_iterator I, class T = std::iter_value_t<I>>
I find_not(I first, const T &value) {
    while (*first != value) {
        ++first;
    }
    return first;
}

export template <std::input_iterator I, std::sentinel_for<I> S, class T = std::iter_value_t<I>>
I find(I first, S last, const T &value) {
    while (first != last) {
        if (*first == value) {
            break;
        }
        ++first;
    }
    return first;
}

export template <std::ranges::input_range R, class I = std::ranges::iterator_t<R>, class T = std::iter_value_t<I>>
I find(R &&range, const T &value) {
    return find(std::ranges::begin(range), std::ranges::end(range), value);
}


export template <std::input_iterator I, std::sentinel_for<I> S, class BinaryOp, class ReduceOp, class T = std::iter_value_t<I>>
    requires returns_t<BinaryOp, T, T, T> && returns_t<ReduceOp, T, T, T> && std::copyable<T>
T reduce(I first1, S last1, I first2, T init, BinaryOp binaryOp, ReduceOp reduceOp) {
    while (first1 != last1) {
        init = reduceOp(init, binaryOp(*first1, *first2));
        ++first1;
        ++first2;
    }
    return init;
}

export template <std::input_iterator I, std::sentinel_for<I> S, class T = std::iter_value_t<I>>
T inner_product(I first1, S last1, I first2) {
    return reduce(first1, last1, first2, T{}, std::multiplies<>{}, std::plus<>{});
}

export template <std::input_iterator I, std::sentinel_for<I> S, class BinaryOp, class ReduceOp, class T = std::iter_value_t<I>>
    requires returns_t<BinaryOp, T, T, T> && returns_t<ReduceOp, T, T, T> && std::copyable<T>
T fast_reduce(I first1, S last1, I first2, T init, BinaryOp binaryOp, ReduceOp reduceOp) {
    if constexpr (std::random_access_iterator<I>) {
        while (last1 - first1 > 4) {
            T aux1 = reduceOp(binaryOp(first1[0], first2[0]), binaryOp(first1[1], first2[1]));
            T aux2 = reduceOp(binaryOp(first1[2], first2[2]), binaryOp(first1[3], first2[3]));
            T aux3 = reduceOp(aux1, aux2);
            init = reduceOp(init, aux3);
            first1 += 4;
            first2 += 4;
        }
    } 
    return reduce(first1, last1, first2, init, binaryOp, reduceOp);
}


export template <std::input_iterator I, std::sentinel_for<I> S, class T = std::iter_value_t<I>>
T fast_inner_product(I first1, S last1, I first2) {
    return fast_reduce(first1, last1, first2, T{}, std::multiplies<>{}, std::plus<>{});
}

