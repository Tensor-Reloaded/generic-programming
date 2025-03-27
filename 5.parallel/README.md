# Parallel algorithms

Parallelization can speed up computation by a large factor and is heavily used in Big Data processing, Machine Learning, and scientific computing. Many fundamental algorithms can be improved by providing a parallel implementation. Many of the STL algorithms have parallel implementations that can be enabled through different execution policies.

The C++ standard defines 4 execution policies (see https://en.cppreference.com/w/cpp/algorithm/execution_policy_tag_t and https://en.cppreference.com/w/cpp/algorithm/execution_policy_tag):
* sequenced_policy (`std::execution::seq`) -> the algorithm must be sequential.
* parallel_policy (`std::execution::par`) -> the algorithm may use thread-level parallelism.
* parallel_unsequenced_policy (`std::execution::par_unseq`) -> the algorithm may use thread-level parallelism and/or vectorization and the algorithm is permitted to execute instructions in an unordered fashion.
* unsequenced_policy  (`std::execution::unseq`) -> the algorithm may be vectorized on a single thread.

Generic algorithms may be parallelized across different devices and retain their implementation. For example, STL algorithms can be parallized on GPU using the nvidia cuda compiler: https://developer.nvidia.com/blog/accelerating-standard-c-with-gpus-using-stdpar/.  


## Usage

On Windows and macOS, MSVC and clang implement their own version of parallel stl, but may use other backends such as oneTBB. For Linux, both g++ and clang require linking either with TBB or OpenMP. 
Install [oneTBB](https://github.com/uxlfoundation/oneTBB) and link with it:

```
sudo apt update
sudo apt install libtbb-dev
# Alternatively, (Intel-only), you can install
# sudo apt install intel-oneapi-tbb
# ...
# mkdir -p build; g++-14 -Wall -Wextra -O2 -std=c++23 main.cpp -o build/main.exe -ltbb
```

On windows 

```c++
// init vec 
std::sort(std::execution::seq, vec.begin(), vec.end()); // sequential sort
// init vec
std::sort(std::execution::par, vec.begin(), vec.end()); // parallel sort
// init vec
std::sort(std::execution::par_unseq, vec.begin(), vec.end()); // parallel + vectorized sort
// init vec
std::sort(std::execution::par, vec.begin(), vec.end()); // vectorized sort
```

A table of possible results would be
| Version  | size    | time       | 
|----------|---------|------------|
| sequential sort       |  1'000'000 | 0.251981s  |
| parallel sort       | 1'000'000 | 0.031534s |
| parallel unsequenced sort std| 1'000'000 | 0.029520s  |
| unsequenced sort       | 1'000'000 | 0.243147s  |
| sequential sort       |  10'000'000 | 2.956876s  |
| parallel sort       | 10'000'000 | 0.429045s |
| parallel unsequenced sort std| 10'000'000 | 0.385953s  |
| unsequenced sort       | 10'000'000 | 2.946679s  |

## Parallel find

```c++
template <std::input_iterator I, std::sentinel_for<I> S, class Pred>
    requires std::indirect_unary_predicate<Pred, I>
I our_find_if(I first, S last, Pred pred) {
    while (first != last) {
        if (pred(*first)) {
            break;
        }
        ++first;
    }
    return first;
}
```

This is our custom find algorithm. We can implement a parallel version if we split the search range into multiple non-overlapping subranges, and split the searching task to multiple threads. 

```c++
// this must be a random access iterator, otherwise we can't use fast traversal and the parallel algorithm would become useless
template <std::random_access_iterator I, std::sentinel_for<I> S, class Pred>
    requires std::indirect_unary_predicate<Pred, I>
I parallel_find_if(I first, S last, Pred pred) {
    static_assert(!std::is_same_v<S, std::unreachable_sentinel_t>,
        "parallel_find_if does not support std::unreachable_sentinel_t as the sentinel type.");
    // normally, this would check the length of the sequence and split it into chunks dynamically using an heuristic
    // based on the length of the sequence and the available cpu cores.
    // it should also accept a threadpool if the user already has one
    auto chunk_size = std::distance(first, last) / 4;
    auto limit1 = std::next(first, chunk_size);
    auto limit2 = std::next(limit1, chunk_size);
    auto limit3 = std::next(limit2, chunk_size);

    // we pass non-everlapping subranges to each thread
    auto thread1 = std::async(std::launch::async, our_find_if<I, S, Pred>, first, limit1, pred);
    auto thread2 = std::async(std::launch::async, our_find_if<I, S, Pred>, limit1, limit2, pred);
    auto thread3 = std::async(std::launch::async, our_find_if<I, S, Pred>, limit2, limit3, pred);
    auto thread4 = std::async(std::launch::async, our_find_if<I, S, Pred>, limit3, last, pred);

    // now we wait and ge the values from each thread
    auto found1 = thread1.get();
    if (found1 != limit1) {
        // we found a value in the first subrange!
        return found1;
    }
    // we didn't find a value in the first subrange
    auto found2 = thread2.get();
    if (found2 != limit2) {
        return found2;
    }
    auto found3 = thread3.get();
    if (found3 != limit3) {
        return found3;
    }
    // we didn't find what we searched for in none of the first 3 subranges
    // this means it's either in the last subrange, or we should return last
    return thread4.get(); 
}
```

Above we have a simple implementation for a parallel find.  
We define 4 non-overlapping subranges, then pass each subrange to a different thread and wait for the search result. 

A table of possible results would be
| Version  | size    | time       | 
|----------|---------|------------|
| Sequential find       |  500 | 0.000001s  |
| Parallel find       | 500 | 0.001907s |
| Sequential find sort std| 500'000'000 | 1.204353s  |
| Parallel find       | 500'000'000 | 0.573431s  |

In theory, we can have a 4x speedup when splitting the work to 4 threads. In practice, the performance improvements depend on the length of the sequence and thread scheduling.

## Other parallel algorithms

Most algorithms applied to random access iterators can be parallelized efficiently provided that the processing elements is independent. Therefore, we can implement parallel versions for `copy`, `transform`, `reduce`, `find`.

**Homework** Implement parallel versions for the aforementioned algorithms and compare their performance. Optional: Use `tbb::task_scheduler_init` and `tbb::task_group` as a thread pool.

