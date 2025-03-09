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
|--------------------------|------------------|------------------|------------------|------------------------|----------------------|----------------------|----------------------|
| `input_iterator`         | ✅               | ✅               | ❌               | ❌                      | ❌                    | ❌                    | ❌                    |
| `output_iterator`        | ❌               | ✅               | ❌               | ❌                      | ❌                    | ❌                    | ❌                    |
| `forward_iterator`       | ✅               | ✅               | ❌               | ❌                      | ✅                    | ❌                    | ❌                    |
| `bidirectional_iterator` | ✅               | ✅               | ✅               | ❌                      | ✅                    | ❌                    | ❌                    |
| `random_access_iterator` | ✅               | ✅               | ✅               | ✅                      | ✅                    | ✅                    | ✅                    |
| `contiguous_iterator`    | ✅               | ✅               | ✅               | ✅                      | ✅                    | ✅                    | ✅                    |

## Searching a range
```
template <std::input_iterator I, std::sentinel_for<I> S, class T>
requires std::equality_comparable_with<std::iter_value_t<I>, T>
I find(I first, S last, const T& value) {
    while (first != last and *first != value) {
        ++first;
    }
    return first;
}

template <std::ranges::input_range R, class T, class I = ranges::iterator_t<R>>
requires std::equality_comparable_with<std::iter_value_t<I>, T>
I find(R range, const T& value) {
    return find(std::ranges::begin(range), std::ranges::end(range), value);
}
```

See https://en.cppreference.com/w/cpp/algorithm/ranges/find. 

## References
* https://en.cppreference.com/w/cpp/header/iterator
