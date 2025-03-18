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


## Binary search

Fast search algorithms such as binary search assume that the range is already sorted. Before talking about sorted ranges, we need to talk about partitioned ranges.

### Partition

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
std::pair<I, N> is_partitioned_n(I first, N n, P pred) {
    auto [first_false, n_false] = find_if_not_n(first, n, pred); // we get the first position which returns false, and how many elements we still have in the range
    auto [_, n_true] = find_if_n(first_false, n_false, pred); // we get the next position which returns true, and how many elements we still have in the remaining range
    return n_true == 0; // if we have no elements in the range, we didn't find any true that is after a false
}
```