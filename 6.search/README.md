# Search algorithms

## Linear search

We have encountered `find` and `find_if` in the previous chapters. We will review them again as they are commonly used and very important building blocks.

```c++
template <std::input_iterator I, std::sentinel_for<I> S, class Pred>
    requires std::indirect_unary_predicate<Pred, I>
I find_if(I first, S last, Pred pred) {
    while (first != last) {
        if (pred(*first)) {
            break;
        }
        ++first;
    }
    return first;
}
```

Another useful `find_if` variant is `find_if_not`. While we can use `find_if` with a negated predicate to simulate `find_if_not`, it is certainly usefull to be able to use a different function with the same predicate.
```c++
template <std::input_iterator I, std::sentinel_for<I> S, class Pred>
    requires std::indirect_unary_predicate<Pred, I>
I find_if_not(I first, S last, Pred pred) {
    while (first != last) {
        if (not pred(*first)) {
            break;
        }
        ++first;
    }
    return first;
}
```

Another possible implementation would be to use `find_if` and negate the predicate.
```c++
template <std::input_iterator I, std::sentinel_for<I> S, class Pred>
    requires std::indirect_unary_predicate<Pred, I>
I find_if_not(I first, S last, Pred pred) {
    return find_if(first, last, std::not_fn(pred));
}
```

There are situations in which it is easier / mandatory to work with counted ranges, meaning we only have a starting point and a count, instead of `first` and `last`. 

```c++
template <std::input_iterator I, std::integral N, class Pred>
    requires std::indirect_unary_predicate<Pred, I>
std::pair<I, N> find_if_n(I first, N n, P pred) {
    while (n) {
        if (pred(*first)) {
            break;
        }
        --n;
        ++first;
    }
    return {first, n};
}

template <std::input_iterator I, std::integral N, class Pred>
    requires std::indirect_unary_predicate<Pred, I>
std::pair<I, N> find_if_not_n(I first, N n, P pred) {
    return find_if_n(first, n, std::not_fn(pred));
}
```
We need to return both `first` and `n` from `find_if_n`, because otherwise the caller will not be able to deduce how far is the found element from the start of the range, and they will not be able to call `find_if_n` again to search for the second element that matches the condition if they lose the returned value of `n`.

What if we want to find the last occurence in a range?
```c++
template <std::input_iterator I, std::sentinel_for<I> S, class Pred>
    requires std::indirect_unary_predicate<Pred, I>
I find_last_if(I first, S last, Pred pred) {
    auto found = last;
    while (first != last) {
        if (pred(*first)) {
            found = first;
        }
        ++first;
    }
    return found;
}
```

**Homework**: Implement `find_last_if_not`, `find_last_if_n`, `find_last_if_not_n`.

### all, any, none

We can use `find_if_not` to check whether all elements of a given range satisfy a predicate by searching for the first element of that range which does not satisfy the predicate. If `find_if_not` returns last, then all elements of that range satify the predicate. We can implement `none_of` and `any_of` in a similar way, the only difference being that we return `true` if we didn't find anything for `none_of`, and `true` if we found something for `any_of`.
```c++
template <std::input_iterator I, std::sentinel_for<I> S, class Pred>
    requires std::indirect_unary_predicate<Pred, I>
bool all_of(I first, S last, Pred pred) {
    return find_if_not(first, last, pred) == last;
}

template <std::input_iterator I, std::sentinel_for<I> S, class Pred>
    requires std::indirect_unary_predicate<Pred, I>
bool none_of(I first, S last, Pred pred) {
    return find_if(first, last, pred) == last;
}

template <std::input_iterator I, std::sentinel_for<I> S, class Pred>
    requires std::indirect_unary_predicate<Pred, I>
bool any_of(I first, S last, Pred pred) {
    return find_if(first, last, pred) != last;
}
```

### Find all

How would we implement `find_all_of`? Let's consider what is different from the previous `find_*` versions. The most important aspect is that we need to return a range of values to the caller. 
```c++
template <std::input_iterator I, std::sentinel_for<I> S, class Pred, class T = std::iter_value_t<I>>
    requires std::indirect_unary_predicate<Pred, I>
std::vector<T> find_all_of_bad_1(I first, S last, Pred pred) {
    std::vector<T> results;
    while (first != last) {
        if (pred(*first)) {
            results.push_back(*first);
        }
        ++first;
    }
    return results;
}
```
This might be the first idea that comes to our mind when implementing `find_all_of`. We return a `std::vector` with all the elements that matched the given predicate. But is this a good generic interface? What about users that need an `std::list` instead of an `std::vector`? What about users that have custom container implementations? What about users that do not have access to heap memory and want everything to be on stack? Using an `std::vector` that allocates memory for `find_all_of` forces all of these users to not use our function as it is not *generic* and *efficient* enough.

```c++
template <std::input_iterator I, std::sentinel_for<I> S, std::output_iterator O, class Pred, class T = std::iter_value_t<I>>
    requires std::indirect_unary_predicate<Pred, I> and std::indirectly_writable<O, T>
template <std::input_iterator I, std::sentinel_for<I> S, class Pred, class T = std::iter_value_t<I>>
    requires std::indirect_unary_predicate<Pred, I>
void find_all_of_bad_2(I first, S last, O output, Pred pred) {
    std::vector<T> results;
    while (first != last) {
        if (pred(*first)) {
            output = *first;
            ++output;
        }
        ++first;
    }
}
```

This implementation is better. The caller can call us with an already allocated output range, with a stack array, with an std::vector and so on, they only need to give us an output iterator. But is it enough? How does the user know whether we found anything, or how many elements did we find?

```c++
template <std::input_iterator I, std::sentinel_for<I> S, std::output_iterator O, class Pred, class T = std::iter_value_t<I>>
    requires std::indirect_unary_predicate<Pred, I> and std::indirectly_writable<O, T>
template <std::input_iterator I, std::sentinel_for<I> S, class Pred, class T = std::iter_value_t<I>>
    requires std::indirect_unary_predicate<Pred, I>
O find_all_of(I first, S last, O output, Pred pred) {
    std::vector<T> results;
    while (first != last) {
        if (pred(*first)) {
            output = *first;
            ++output;
        }
        ++first;
    }
    return output;
}
```

The interface is very important in generic programming for efficiency and composability.
```c++
std::vector<int> v1 = {1, 2, 3, 4, 5};
std::vector<int> v2 = {6, 7, 8, 9, 10};
std::array<int, 5> even; // we allocate a stack array with 5 elements.
std::vector<int> even_heap;

auto is_even = [](int x) {return x % 2;};

auto last_event = find_all_of(v1.begin(), v1.end(), even.begin(), is_even);
last_event = find_all_of(v2.begin(), v2.end(), last_even, is_even);
// between even.begin() and last_even we have all 5 even elements.
// even = [2, 4, 6, 8, 10]

find_all_of(v1.begin(), v1.end(), std::back_inserter(even_heap), is_even); 
// this writes to even_heap and grows it as needed on heap
find_all_of(v2.begin(), v2.end(), std::back_inserter(even_heap), is_even);
// even_heap = [2, 4, 6, 8, 10]
auto last_even_heap = find_all_of(v2.begin(), v2.end(), even_heap.begin(), is_even);
// even_heap = [6, 8, 10, 8, 10]
// between even_heap.begin() and last_even_heap we have [6, 8, 10]
```


## Binary search

Fast search algorithms such as binary search assume that the range is already sorted. Before talking about sorted ranges, we need to talk about partitioned ranges.

### Partition point

A range is partitioned for a given predicate if it can be splitted into two subranges, one for which the predicate is false, and the other for which the predicate is true.

```c++
template <std::input_iterator I, std::sentinel_for<I> S, class Pred>
    requires std::indirect_unary_predicate<Pred, I>
bool is_partitioned(I first, S last, Pred pred) {
    // we assume that the range starts with true
    I first_false = find_if_not(first, last, pred); // we get the first position which returns false;
    I next_true = find_if(first_false, last, pred); // we search for the next true that appears after a false;
    // if we found a true after a false, (next_true != last), the range is not partitioned
    return next_true == last;
}
```

This version of `is_partitioned` assumes that the elements for which the predicate returns `true` should be situated before the elements for which the predicate returns `false`. 

**Food for thought**: Is this the correct (natural) way of defining a partition?


Similarly, we can implement a function that checks whether a counted range is a partition or not.

```c++
template <std::input_iterator I, std::integral N, class Pred>
    requires std::indirect_unary_predicate<Pred, I>
bool is_partitioned_n(I first, N n, P pred) {
    auto [first_false, n_false] = find_if_not_n(first, n, pred);
    // we get the first position which returns false, and how many elements we still have in the range
    auto [_, n_true] = find_if_n(first_false, n_false, pred);
    // we get the next position which returns true, and how many elements we still have in the remaining range
    return n_true == 0; // if we have no elements in the range, we didn't find any true that is after a false
}
```
Now, assuming that a range is partitioned using a predicate, we can search for the partition point using `find_if` and the predicate in linear time. Can we do it in logarithmic time? 
We will start implementing `partition_point_n` that will search for the partition point in a counted range. We implement the algorithm for counted ranges first because it is easier to understand.
```c++
// First, let's define a helper function we will use later.
template <std::forward_iterator I>
I next(I first, std::uint64_t n) {
    while (n != 0) {
        ++first;
        ++n;
    }
    return first;
}

template <std::forward_iterator I, std::integral N, class Pred>
    requires std::indirect_unary_predicate<Pred, I>
I partition_point_n_bad(I first, N n, P pred) {
    // precondition: is_partitioned_n(first, n, pred) is true, but we can't check for that without traversing the whole range.
    // so we just believe the caller knows what they're doing.
    // and they should, otherwise we would not be able to write efficient code.
    while (n != 0) {
        N half = n / 2;
        I mid = next(first, half);
        if (pred(*mid)) {
            first = next(mid, 1);
            n -= half + 1;
        } else {
            n = half;
        }
    }
    return first; // this is now the partition point!
}
```

**Question**: Why is `I` a forward iterator and not an input iterator?

**Question**: Why is this partition_point_n bad?

What if all the elements in the range satisfy the predicate? This is a valid partition, and the output of the function is `first + n` which is the first element beyond the range. What if we searched for the partition point in order to do a linear search in the second partition of the range? How can we do so when we don't know how many elements are in the second partition?

```c++
template <std::forward_iterator I, std::integral N, class Pred>
    requires std::indirect_unary_predicate<Pred, I>
std::pair<I, N> partition_point_n(I first, N n, P pred) {
    // precondition: is_partitioned_n(first, n, pred) is true, but we can't check for that without traversing the whole range.
    // so we just believe the caller knows what they're doing.
    // and they should, otherwise we would not be able to write efficient code.
    while (n != 0) {
        N half = n / 2;
        I mid = next(first, half);
        if (pred(*mid)) {
            first = next(mid, 1);
            n -= half + 1;
        } else {
            n = half;
        }
    }
    return {first, n}; // this is now the partition point!
}
```

Those familiar with `binary_search` will notice that `partition_point_n` (at least the bad version) is identical with a well-written binary search. Questions:
* is it faster than `find_if`?
* can it ever be faster than `find_if` for forward iterators (linked lists)?

***

As a rule of thumb, only tree structures or contiguous ranges with random access operators permit finding elements in `O(log(n))`. Therefore, we just need to write a different implementation for `next` with random access iterators.
```c++
template <std::random_access_iterator I>
I next(I first, std::uint64_t n) {
    return first + n;
}
```

Now that we have implemented `partition_point_n`, let's implement `partition_point`.
```c++
template <std::input_iterator I, std::sentinel_for<I> S, class Pred>
    requires std::indirect_unary_predicate<Pred, I>
I partition_point(I first, S last, Pred pred) {
    // return partition_point_n(first, std::distance(first, last), pred).first;
    auto n = std::distance(first, last);
    while (n != 0) {
        N half = n / 2;
        I mid = next(first, half);
        if (pred(*mid)) {
            first = next(mid, 1);
            n -= half + 1;
        } else {
            n = half;
        }
    }
    return first;
}
```

**Question**: Can we use `partition_point` with unreachable_sentinel?  

### Back to binary search


```c++
template <std::input_iterator I, std::sentinel_for<I> S, class T = std::iter_value_t<I>>
bool binary_search(I first, S last, const T& value) {
    auto pred = [&value](const T& other) {
        return other < value;
    };
    // we required the range to be partitioned in therms of pred
    I found = partition_point(first, last, pred);
    // return found != last and *found == value;
    if (found != last and *found == value) {
        return true;
    }
    return false;
}
```

When we search for an element, we no not require that the range is fully sorted. The algorithm works even if some values are out of order as long as the range is partitioned in terms of `< value`. But if we want our algorithm to work for any `value`, we need the range to be sorted.

Now that we have implemented binary search, are we done? Let's say we searched for `5`, and if we found it we want to remove it. Can we do it? Let's say we want to search for the last aparition of 5, and increase it to 6. Can we do it? 

**Homework**: Implement `lower_bound` and `upper_bound` and return what needs to be returned to propagate efficiency back to the caller. Also include a custom comparator (besides `<`).  
```c++
template <std::input_iterator I, std::sentinel_for<I> S, class Comp, class T = std::iter_value_t<I>>
    requires std::indirect_binary_predicate<Comp, I, I>
```

