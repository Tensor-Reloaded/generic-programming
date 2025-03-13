#pragma once

#include <concepts>
#include <ranges>

template <std::input_iterator I, std::sentinel_for<I> S, std::weakly_incrementable O>
    requires std::indirectly_copyable<I, O>
O copy(I first, S last, O output) {
    while (first != last) {
        *output = *first;
        ++first;
        ++output;
    }
    return output;
}

template <std::ranges::input_range R, std::weakly_incrementable O>
    requires std::indirectly_copyable<std::ranges::iterator_t<R>, O>
O copy(R &&range, O output) {
    return copy(std::ranges::begin(range), std::ranges::end(range), output);
}

template <std::input_iterator I, std::sentinel_for<I> S, std::weakly_incrementable O, class Fn, class T = std::iter_value_t<I>>
    requires std::indirectly_writable<O, std::invoke_result_t<Fn, T>>
O transform(I first, S last, O output, Fn fn) {
    while (first != last) {
        *output = fn(*first);
        ++first;
        ++output;
    }
    return output;
}

template <std::ranges::input_range R, std::weakly_incrementable O, class Fn, class I = std::ranges::iterator_t<R>, class T = std::iter_value_t<I>>
    requires std::indirectly_writable<O, std::invoke_result_t<Fn, T>>
O transform(R &&range, O output, Fn fn) {
    return transform(std::ranges::begin(range), std::ranges::end(range), output, fn);
}

template <class T>
concept reducible = requires(T x, T y) {
    requires std::is_default_constructible_v<T> and std::copyable<T> and std::movable<T>;
    x += y;
    x = x + y;
    x = std::move(y);
};

template <class Fn, class T, class... Args>
concept returns_t = std::regular_invocable<Fn, Args...> and std::same_as<std::invoke_result_t<Fn, Args...>, T>;

template <std::input_iterator I, std::sentinel_for<I> S, class ReduceOp, class T = std::iter_value_t<I>>
    requires returns_t<ReduceOp, T, T, T>
T reduce(I first, S last, T init, ReduceOp fn) {
    while (first != last) {
        init = fn(init, *first);
        ++first;
    }
    return init;
}

template <std::input_iterator I, std::sentinel_for<I> S, class T = std::iter_value_t<I>>
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

template <std::input_iterator I, std::sentinel_for<I> S, class T = std::iter_value_t<I>>
I find(I first, S last, const T &value) {
    while (first != last) {
        if (*first == value) {
            break;
        }
        ++first;
    }
    return first;
}

template <std::ranges::input_range R, class I = std::ranges::iterator_t<R>, class T = std::iter_value_t<I>>
I find(R &&range, const T &value) {
    return find(std::ranges::begin(range), std::ranges::end(range), value);
}

template <std::input_iterator I, std::sentinel_for<I> S, class BinaryOp, class ReduceOp, class T = std::iter_value_t<I>>
    requires returns_t<BinaryOp, T, T, T> and returns_t<ReduceOp, T, T, T>
T reduce(I first1, S last1, I first2, T init, BinaryOp binaryOp, ReduceOp reduceOp) {
    while (first1 != last1) {
        init = reduceOp(init, binaryOp(*first1, *first2));
        ++first1;
        ++first2;
    }
    return init;
}

template <std::input_iterator I, std::sentinel_for<I> S, class T = std::iter_value_t<I>>
T inner_product(I first1, S last1, I first2) {
    return reduce(first1, last1, first2, T{}, std::multiplies<>{}, std::plus<>{});
}

template <std::input_iterator I, std::sentinel_for<I> S, class BinaryOp, class ReduceOp, class T = std::iter_value_t<I>>
    requires returns_t<BinaryOp, T, T, T> and returns_t<ReduceOp, T, T, T>
T fast_reduce(I first1, S last1, I first2, T init, BinaryOp binaryOp, ReduceOp reduceOp) {
    if constexpr (std::random_access_iterator<I>) {
        while (std::distance(first1, last1) > 4) {
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

template <std::input_iterator I, std::sentinel_for<I> S, class T = std::iter_value_t<I>>
T fast_inner_product(I first1, S last1, I first2) {
    return fast_reduce(first1, last1, first2, T{}, std::multiplies<>{}, std::plus<>{});
}

template <std::input_iterator I, std::sentinel_for<I> S, class BinaryOp, class ReduceOp, class T = std::iter_value_t<I>>
    requires returns_t<BinaryOp, T, T, T> and returns_t<ReduceOp, T, T, T>
T faster_reduce(I first1, S last1, I first2, T init, BinaryOp binaryOp, ReduceOp reduceOp) {
    if constexpr (std::random_access_iterator<I>) {
        while (std::distance(first1, last1) > 8) {
            T aux1 = reduceOp(binaryOp(first1[0], first2[0]), binaryOp(first1[1], first2[1]));
            T aux2 = reduceOp(binaryOp(first1[2], first2[2]), binaryOp(first1[3], first2[3]));
            T aux3 = reduceOp(binaryOp(first1[4], first2[4]), binaryOp(first1[5], first2[5]));
            T aux4 = reduceOp(binaryOp(first1[6], first2[6]), binaryOp(first1[7], first2[7]));
            T aux5 = reduceOp(aux1, aux2);
            T aux6 = reduceOp(aux3, aux4);
            T aux7 = reduceOp(aux5, aux6);
            init = reduceOp(init, aux7);
            first1 += 8;
            first2 += 8;
        }
    }
    return reduce(first1, last1, first2, init, binaryOp, reduceOp);
}

template <std::input_iterator I, std::sentinel_for<I> S, class T = std::iter_value_t<I>>
T faster_inner_product(I first1, S last1, I first2) {
    return faster_reduce(first1, last1, first2, T{}, std::multiplies<>{}, std::plus<>{});
}
