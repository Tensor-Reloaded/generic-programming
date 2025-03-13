# The minmax algorithm

## Finding the minimum of 2 elements

Many of the most important algorithms in computer science are based on essential building blocks, such as determining the minimum of two elements, swapping two elements. Determining the minimum of two elements requires we have a [strict weak ordering](https://en.cppreference.com/w/cpp/concepts/strict_weak_order) relation defined on our types. Based on this, we can write the following generic code:
```c++
template <class T, class Compare>
    requires std::strict_weak_order<Compare, T, T>
inline const T& min(const T& a, const T& b, Compare comp) {
    if (comp(b, a)) {
        return b;
    }
    return a;
} 
```

Similarly, we can write max:
```c++
template <class T, class Compare>
    requires std::strict_weak_order<Compare, T, T>
inline const T& max(const T& a, const T& b, Compare comp) {
    if (comp(b, a)) {
        return a;
    }
    return b;
} 
```

And we can write the default `min` and `max` as follows:
```c++
template <class T>
    requires std::strict_weak_order<std::less<T>, T, T>
inline const T& min(const T& a, const T& b) {
    return min(a, b, std::less<T>{}});
}

template <class T>
    requires std::strict_weak_order<std::less<T>, T, T>
inline const T& max(const T& a, const T& b) {
    return max(a, b, std::less<T>{}});
}
```

## Finding the minimum of `n` elements

Finding the minimum and maximum of a range of `n` elements can be easily implemented using iterators. Unlinke the minimum of two elements, in a range we can also have `0` elements, therefore we return an iterator instead of a const reference. If we receive an empty range (`first == last`), we return the last iterator, otherwise we return an iterator pointing to the **first minimum** or **last maximum** element. These aspects are very important, otherwise we would not be able to use these functions when building stable algorithms.
```c++
template <class I, class Compare, class T = std::iter_value_t<I>>
    requires std::strict_weak_order<Compare, T, T>
I min_element(I first, I last, Compare comp) {
    if (first == last) {
        return last;
    }
    I minimum = first;
    while (++first != last) {
        if (comp(*first, *minimum)) {
            minimum = first;
        }
    }
    return minimum;
}

template <class I, class Compare, class T = std::iter_value_t<I>>
    requires std::strict_weak_order<Compare, T, T>
I max_element(I first, I last, Compare comp) {
    if (first == last) {
        return last;
    }
    I maximum = first;
    while (++first != last) {
        if (not comp(*first, *maximum)) {
            maximum = first;
        }
    }
    return maximum;
}
```

## Finding both the minimum and the maximum

The naive implementation of `minmax_element` would call both `min_element` and `max_element`:
```c++
template <class I, class Compare, class T = std::iter_value_t<I>>
    requires std::strict_weak_order<Compare, T, T>
std::pair<I, I> naive_minmax_element(I first, I last, Compare comp) {
    return {min_element(first, last, comp), max_element(first, last, comp)};
}
```
This version would do `2n` comparisons. We can write a faster version that will do `3n \ 2` comparisons.
```c++
template <class I, class Compare, class T = std::iter_value_t<I>>
    requires std::strict_weak_order<Compare, T, T>
std::pair<I, I> minmax_element(I first, I last, Compare comp) {
    // First we need to take care of the base cases, empty range or one element in range
    if (first == last) {
        // empty range
        return {last, last};
    }
    I minimum = first;
    ++first;
    if (first == last) {
        // one element in range
        return {minimum, minimum};
    }
    I maximum = first;
    ++first;
    // now we have to put minimum and maximum in correct order
    if (comp(*minimum, *maximum)) {
        std::swap(minimum, maximum);
    }
    while (fist != last and std::next(first) != last) {
        I potential_min = first;
        I potential_max = std::next(first);
        // now we have to put potential_min and potential_max in correct order
        if (comp(*potential_max, *potential_min)) {
            std::swap(potential_max, potential_min);
        }

        // now we only check the potential min with the minimum, and potential max with the maximum
        if (comp(*potential_min, *minimum)) {
            minimum = potential_min;
        }
        if (not comp(*potential_max, *maximum)) {
            maximum = potential_max;
        }
        
        // i += 2
        ++first;
        ++first;
    }
    // n is odd
    if (first != last) {
        if (not comp(*first, *maximum)) {
            maximum = first;
        }
        if (comp(*first, *minimum)) {
            minimum = first;
        }
    }
    return {minimum, maximum};
}
```

Comparint built-in types is not expensive, however comparing strings and entire ranges is expensive and minimizing the number of comparisons is needed for longer sequences.