#include <concepts>
#include <iterator>
#include <future>

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

// this must be a random access iterator, otherwise we can't use fast traversal and the parallel algorithm would become useless
template <std::random_access_iterator I, std::sentinel_for<I> S, class Pred>
    requires std::indirect_unary_predicate<Pred, I>
I parallel_find_if(I first, S last, Pred pred) {
    // normally, this would check the length of the sequence and split it into chunks dynamically using an heuristic based on the length of the sequence and the available cpu cores.
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