// include what you need here
#include <print>

// then import modules
import my_module;

int main() {
    std::println("I am printing a line!");

    // now we can use functions exported in the module
    my_namespace::old_hello_world();
    my_namespace::new_hello_world();
    // my_namespace::private_hello_world(); // error: ‘private_hello_world’ is not a member of ‘my_namespace’
}
