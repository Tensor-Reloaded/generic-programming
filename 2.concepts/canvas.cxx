#include <concepts> // concept
#include <cstdint> // std::uint32_t
#include <print> // std::println
#include <string> // std::string
#include <vector> // std::vector

template <class T>
concept Canvas = requires(T a) {
    a.display();
    { a.clear() } -> std::same_as<void>;
};

template <class T, class CanvasType>
concept Drawable = requires(T a, CanvasType b) {
    requires Canvas<CanvasType>;
    { a.draw(b) } -> std::same_as<void>;
};

struct MyCanvas {
    void display() const {
        std::println("<canvas>");
        for (const auto &item : drawingBoard) {
            std::println("<item>{}</item>", item);
        }
        std::println("</canvas>");
    }
    void clear() noexcept {
        drawingBoard.clear();
    }

    std::vector<std::string> drawingBoard;
};

// This example can be further improved by transforming Circle into a template class that can be drawn on multiple kind of canvases.
struct Circle {
    std::uint32_t radius = 0;
    void draw(MyCanvas &canvas) const {
        canvas.drawingBoard.push_back("Circle with radius " + std::to_string(radius));
    }
};

template <Canvas C, Drawable<C> D>
void draw_on_canvas(C &canvas, D &drawable) {
    drawable.draw(canvas);
}

int main() {
    auto canvas = MyCanvas{};
    auto circle_1 = Circle{1};
    auto circle_2 = Circle{2};

    draw_on_canvas(canvas, circle_1);
    draw_on_canvas(canvas, circle_2);
    draw_on_canvas(canvas, circle_1);

    // draw_on_canvas(circle_2, circle_1);
    // This will not compile. Error messages include:
    // ... in requirements with ‘T a’ [with T = Circle] ... the required expression ‘a.display()’ is invalid
    // ... the required expression ‘a.clear()’ is invalid

    canvas.display();

    std::println("\nClearing canvas");
    canvas.clear();
    canvas.display();
    return 0;
}
