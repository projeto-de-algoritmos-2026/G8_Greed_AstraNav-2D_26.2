#include "render/window.hpp"

namespace astranav::render {
    Window::Window(int width, int height, const std::string& title) {
        InitWindow(width, height, title.c_str());
        SetTargetFPS(60);
    }

    Window::~Window(){
        CloseWindow();
    }

    bool Window::should_close() const { return WindowShouldClose(); }
    void Window::begin_drawing() const { BeginDrawing(); }
    void Window::end_drawing() const { EndDrawing(); }
    void Window::clear_background(Color color) const { ClearBackground(color); }

    void Window::draw_cargo_hud(const algorithms::CargoResult& cargo, float x, float y) const {
        DrawRectangle(x, y, 320, 400, Fade(DARKGRAY, 0.0f));
        DrawRectangleLines(x, y, 320, 400, LIGHTGRAY);
        DrawText("MANIFESTO DE CARGA", x + 20, y + 15, 20, RAYWHITE);

        float current_y = y + 50;

        for (const auto& item : cargo.loaded_items) {
            Color item_color = (item.fraction_taken < 1.0) ? ORANGE : LIME;

            DrawRectangle(x + 15, current_y, 290, 40, Fade(item_color, 0.4f));
            DrawRectangleLines(x + 15, current_y, 290, 40, item_color);

            const char* text = TextFormat("%s, (%.0f%%)", item.name.c_str(), item.fraction_taken * 100);
            DrawText(text, x + 25, current_y + 10, 20, WHITE);

            current_y += 45;
        }

        DrawText(TextFormat("Peso Usado: %.1f kg", cargo.total_weight), x + 20, current_y + 20, 20, LIGHTGRAY);
        DrawText(TextFormat("Retorno Científico: %.1f", cargo.total_scientific_return), x + 20, current_y + 50, 20, GREEN);
    }
}