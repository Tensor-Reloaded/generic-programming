#include <concepts> // concept
#include <print>    // std::print
#include <string>   // std::string


template <class T, class U = T>
concept addable = requires(T a, U b) {
    a + b;
    b + a; // if we do not include this, we are not guaranteed that addition on T is commutative
};

addable auto add_addables(addable auto a, addable auto b) {
    return a + b;
}

struct NotAddable {
    int x = 0;
};

int main() {
    std::print("{}\n", add_addables(5, 4));
    std::print("{}\n", add_addables(5.4, 4));
    std::print("{}\n", add_addables(std::string("Hello "), std::string("world")));
    std::print("{}\n", add_addables(std::string("Hello "), 'W'));
    std::print("{}\n", add_addables<std::string, char>(std::string("Hello "), 'W'));

    // add_addables(NotAddable{1}, NotAddable{5});  // the required expression ‘(a + b)’ is invalid

    return 0;
}