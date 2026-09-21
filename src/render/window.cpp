#include "render/window.hpp"

namespace astranav::render {
    Window::Window(int width, int height, const std::string& title) {
        InitWindow(width, height, title.c_str());
        SetTargetFPS(60);
    }

    Window::~Window(){
        CloseWindow();
    }

    bool Window::should_close() const {
        return WindowShouldClose();
    }

    void Window::begin_drawing() const {
        BeginDrawing();
    }

    void Window::end_drawing() const {
        EndDrawing();
    }

    void Window::clear_background(Color color) const {
        ClearBackground(color);
    }
}