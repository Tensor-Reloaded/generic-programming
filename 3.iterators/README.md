# Iterators

Iterators provide a standardized way to traverse and manipulate elements in a data structure. 
They are used in generic programming, allowing algorithms to work with different container types seamlessly. 
Iterators are analogous to coordinates as they indicate positions within a sequence. 


Just like in mathematics, where a semi-open range `[a, b)` includes the starting point `a` but excludes `b`, iterators follow the same principle with `[first, last)`. 
This ensures that iteration stops precisely at `last`, preventing out-of-bounds access and making empty ranges naturally expressible when `first == last`. 
This approach simplifies loop conditions and improves consistency across algorithms. Most algorithms working with iterators look like this:
```
// function declaration
while(first != last) {
    // process *first
    ++first;
}
```

## Iterator types
| Iterator                 | Description |
|--------------------------|-------------|
| `input_iterator`         | Can read referenced values and be incremented. |
| `output_iterator`        | Can write values and be incremented. |
| `forward_iterator`       | An `input_iterator` that supports equality comparison and multi-pass. |
| `bidirectional_iterator` | A `forward_iterator` that supports movement backwards. |
| `random_access_iterator` | A `bidirectional_iterator` that supports constant-time advancement and subscripting. |
| `contiguous_iterator`    | A `random_access_iterator` that refers to contiguous memory elements. |

## Feature support

| Iterator                 | `*`  | `++` | `--`  | `+= n` | `==`  | `<`  | `[]`  |
|--------------------------|------------------|------------------|-----------------|------------------------|----------------------|----------------------|----------------------|
| `input_iterator`         | ✅               | ✅  (all copies are invalidated) | ❌              |   ❌             | ❔                  | ❌                   | ❌                    |
| `output_iterator`        | ❔ (can only be used once during assignment)               | ✅              | ❌              | ❌                    | ❌                  | ❌                   | ❌                    |
| `forward_iterator`       | ✅               | ✅              | ❌              | ❌                    | ✅                  | ❌                   | ❌                    |
| `bidirectional_iterator` | ✅               | ✅              | ✅              | ❌                    | ✅                  | ❌                   | ❌                    |
| `random_access_iterator` | ✅               | ✅              | ✅              | ✅                    | ✅                  | ✅                   | ✅                    |
| `contiguous_iterator`    | ✅               | ✅              | ✅              | ✅                    | ✅                  | ✅                   | ✅                    |


## Usage

### Copying
We can use iterators to implement algorithms working on containers, ranges, and even views. Let's consider the `copy` algorithm.
```
template<std::input_iterator I, std::sentinel_for<I> S, std::weakly_incrementable O>
    requires std::indirectly_copyable<I, O>
O copy(I first, S last, O output) {
    while (first != last) {
        *output = *first;
        ++first;
        ++output;
    }
    return output;
}
```
This algorithm copies the range of elements between `[first, last)` to the output iterator `output`. While most constraints are specified using compile time concepts, this algorithm also requires that the output iterator `output` is valid and incrementable until `first == last` (memory is reserved and we can copy). This means that `output` should be an iterator from a range with size greater or equal to `std::distance(first, last)`, or `output` is an iterator adaptor that can grow its own range as needed.

When writing generic functions, it is always a good idea to return everything our caller may need. This is why we return `output`, which is now the coordinate right after the last value from `[first, last)` was copied to the output range.

We can also write a version of `copy` that receives a full range as an input.
```
template<std::ranges::input_range R, std::weakly_incrementable O>
    requires std::indirectly_copyable<std::ranges::iterator_t<R>, O>
O copy(R&& range, O output) {
    return copy(std::ranges::begin(range), std::ranges::end(range), output);
}
```

### Transforming
If instead of copying, we want to transform elements, we can write a `transform` function:
```
export template <std::input_iterator I, std::sentinel_for<I> S, std::weakly_incrementable O, class Fn, class T = std::iter_value_t<I>>
    requires std::indirectly_writable<O, std::invoke_result_t<Fn, T>>
O transform(I first, S last, O output, Fn fn) {
    while (first != last) {
        *output = fn(*first);
        ++first;
        ++output;
    }
    return output;
}

template<std::ranges::input_range R, std::weakly_incrementable O, class Fn, class I = std::ranges::iterator_t<R>, class T = std::iter_value_t<I>>
    requires std::indirectly_writable<O, std::invoke_result_t<Fn, T>>
O transform(R&& range, O output, Fn fn) {
    return transform(std::ranges::begin(range), std::ranges::end(range), output, fn);
}
```

Usage example:
```
auto vec = std::vector{1, 2, 3, 4, 5};
auto another = std::vector<int>(vec.size(), 0); // another is  {0, 0, 0, 0, 0}
transform(vec, another.begin(), [](int x) {
    if (x % 2 == 0) {
        return x * 2;
    }
    return x - 1;
});
// another is now {0, 4, 2, 8, 4}
```
```
auto vec = std::vector{1, 2, 3, 4, 5};
transform(vec, vec.begin(), [](int x) {
    return x * x;
});
// vec is now {1, 4, 9, 16, 25}
```


**Homework**: Implement `copy_if` and `transform_if`.

### Linear searching
```
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

template <std::ranges::input_range R, class I = ranges::iterator_t<R>, class T = std::iter_value_t<I>>
I find(R&& range, const T& value) {
    return find(std::ranges::begin(range), std::ranges::end(range), value);
}
```

Consider the following code:
```
constexpr auto size = 500'000;
// vec is a vector with a random permutation of all elements between [0, size)
for (int i = 0; i < size; ++i) {
    auto found = find(vec.begin(), vec.end(), i);
    process(found);
}

// we know that the element we search for is always in the range.
// Find checks for first != last to see if we haven't finished the range, but this condition is always false.
// Can't we use a slightly faster find version? The answer is yes.
for (int i = 0; i < size; ++i) {
    auto found = find(vec.begin(), std::unreachable_sentinel, i); 
    // first != std::unreachable_sentinel will always return true => the compiler optimizes it
    process(found);
}
```

A table of possible results would be
| Version  | size    | time       | 
|----------|---------|------------|
| v1       | 100'000 | 0.679640s  |
| v2       | 100'000 | 0.554762s  |
| v1       | 500'000 | 17.394937s |
| v2       | 500'000 | 13.989906s |

**Homework**: Implement `find_if` and `find_all`. 

### Inner product

First, we define a concept that will help us in specifying the requirements for the function:
```
template <class Fn, class T, class... Args>
concept returns_t = std::regular_invocable<Fn, Args...> and std::same_as<std::invoke_result_t<Fn, Args...>, T>;
```

Now we define a reduce function that accepts two ranges, a binary operation that combines elements from each range, and a reduce operation that reduces everything to a single value.
```
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
```

Now we can write our inner product between two ranges using the `reduce` we defined above.

```
template <std::input_iterator I, std::sentinel_for<I> S, class T = std::iter_value_t<I>>
T inner_product(I first1, S last1, I first2) {
    return reduce(first1, last1, first2, T{}, std::multiplies<>{}, std::plus<>{});
}
```

This function will take two vectors, multiply the values element-wise and sum them. Can we write a faster version?
```
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
```
Calculating the distance between two iterators is `O(n)` for forward and bidirectional iterators, and `O(1)` for random access iterators. When our iterators allows random access, we can specialize the algorithms to take advantage of this feature and perform less binary and reduce operations.

Can we write a faster version?
```
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
            first1 += 4;
            first2 += 4;
        }
    }
    return reduce(first1, last1, first2, init, binaryOp, reduceOp);
}

template <std::input_iterator I, std::sentinel_for<I> S, class T = std::iter_value_t<I>>
T faster_inner_product(I first1, S last1, I first2) {
    return faster_reduce(first1, last1, first2, T{}, std::multiplies<>{}, std::plus<>{});
}
```

A table of possible results would be
| Version  | size    | time       | 
|----------|---------|------------|
| inner_product       | 1'000'000'000 | 1.420974s  |
| fast_inner_product       | 1'000'000'000 | 1.121097s |
| faster_inner_product       | 1'000'000'000 | 1.075829s  |

## References
* https://en.cppreference.com/w/cpp/header/iterator
