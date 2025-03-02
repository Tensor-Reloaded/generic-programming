# Concepts

See https://en.wikipedia.org/wiki/Concepts_(C%2B%2B).
***

Writing generic code is usually handled in OOP by implementing a function in terms of a base object that needs to be inherited. For example:
```
// BaseCanvas and Drawable are pure abstract classes.
void draw_on_canvas(BaseCanvas& baseCanvas, Drawable& drawable) {
    drawable.draw(baseCanvas);
}
```
This function will compile for any derived types but may fail at runtime if the `draw` method is not overridden.
While this is a reasonable approach, substitution with derived types is done at runtime. If explicit type identification is required, RTTI introduces further costs. Moreover, the function call incurs overhead due to the pointer indirection required by the `vfptr`. Furthermore, using the OOP approach becomes messy when a type needs to derive from multiple base classes, such as being both drawable, serializable, and so on.
***
Instead of dynamic polymorphism and runtime function dispatch, C++ offers a powerful compile-time tool: templates. Templates in C++ are used to create generic functions and classes that can operate on any type of data. Using templates, the above function can be rewritten as:
```
void draw_on_canvas(auto& canvas, auto& drawable) {
    drawable.draw(canvas);
}

// Using auto is equivalent to the following: 
template<class Canvas, class Drawable>
void draw_on_canvas(Canvas& canvas, Drawable& drawable) {
    drawable.draw(canvas);
}
```
This function can take any types, and compilation will fail if the `Drawable` type does not implement a `draw` method that accepts a `Canvas`. The substitution is done at compile time, so we do not incur any overhead due to virtual function call indirection.

***
Templates are often considered hard to use and "black magic" due to unreadable compile errors and cumbersome syntax. Modern C++ introduced concepts to enhance generic programming.
Concepts extend templates by providing powerful predicate evaluation features for template parameters, enforcing compile-time constraints and providing better error messages.

There are many useful concepts provided by the standard library, and we can define custom concepts as follows:
```
// We define the Canvas concept. Let's assume that canvases need to have a display method, 
// a resize method that accepts a width and a height, and a clear method.

template <class T>
concept Canvas = requires (T a, std::uint32_t width, std::uint32_t height) {
    a.display();
    a.resize(width, height); // We force the resize method to accept two uint32_t. 
    { a.clear() } -> std::same_as<void>; // We force the clear method to return void.
};
// Using the Canvas concept, we can force the compiler to raise an error early 
// whenever a type does not satisfy the concept. 

template <class T, class CanvasType>
concept Drawable = requires(T a, CanvasType b) {
    requires Canvas<CanvasType>; // We force CanvasType to satisfy the Canvas concept.
    { a.draw(b) } -> std::same_as<void>;
};
// Using the Drawable concept forces types to implement a draw method 
// that accepts a type satisfying the Canvas concept.
```
We can use the defined concepts as follows:
```
// Drawable is a concept that depends on the canvas, therefore we must explicitly specify
// decltype(canvas) as the template argument for Drawable to ensure that drawable is
// constrained to a type that can draw on the given canvas.
void draw_on_canvas(Canvas auto& canvas, Drawable<decltype(canvas)> auto& drawable) {
    drawable.draw(canvas);
}

// Alternatively, we can use
template<Canvas C, Drawable<C> D>
void draw_on_canvas(C& canvas, D& drawable) {
    drawable.draw(canvas);
}

// This version uses classic templates and checks the constraints using static_assert,
// providing useful error messages. 
template<class C, class D>
void draw_on_canvas(C& canvas, D& drawable) {
    static_assert(Canvas<C>, "C must be a Canvas");
    static_assert(Drawable<D, C>, "D must be a Canvas");
    drawable.draw(canvas);
}
```

See the [canvas.cxx](./canvas.cxx) file for a full example.
***
See the [addable.cxx](./addable.cxx) file for another simple example.
***
## How to run
```
make canvas
make addable
make clean
```
* Check the [Makefile](./Makefile)

## References
* https://en.cppreference.com/w/cpp/concepts
* https://en.cppreference.com/w/cpp/language/constraints
