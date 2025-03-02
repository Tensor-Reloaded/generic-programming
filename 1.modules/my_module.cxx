module;
// Here we can use #include
#include <iostream>
#include <print>

export module my_module;
// From here on we can't use include

namespace my_namespace {

std::string private_hello_world() {
    return "Hello world";
}

export void old_hello_world() {
    std::cout << private_hello_world() << '\n';
}

export void new_hello_world() {
    std::println("{}", private_hello_world());
    std::print("I can use {} decimals when printing floats: {:.3f}\n", 3, 0.987654321);
}

} // namespace my_namespace
