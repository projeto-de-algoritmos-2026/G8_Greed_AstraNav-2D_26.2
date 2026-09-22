#include "render/window.hpp"
#include <algorithm>

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

    void Window::draw_flight_plan(const algorithms::RoutePlan& plan, const std::vector<algorithms::Outpost>& all_outposts, double total_distance, float y_pos) const {
        float start_x = 420.0f;
        float end_x = 1200.0f;
        float width = end_x - start_x;

        /* Linha da Trajetoria */
        DrawLineEx({start_x, y_pos}, {end_x, y_pos}, 2.0f, Fade(LIGHTGRAY, 0.4f));
        
        /* Terra */
        DrawCircleV({start_x, y_pos}, 12.0f, BLUE);
        DrawText("Terra", start_x - 15, y_pos + 20, 18, RAYWHITE);

        /* Destino */
        DrawCircleV({end_x, y_pos}, 16.0f, PURPLE);
        DrawText("Destino", end_x - 30, y_pos + 25, 18, RAYWHITE);

        for (const auto& outpost : all_outposts) {
            float pos_x = start_x + (outpost.distance_from_earth / total_distance) * width;

            bool is_stop = std::ranges::any_of(plan.stops_made,
                [&](const auto& stop) { return stop.name == outpost.name; });

            Color node_color = is_stop ? YELLOW : DARKGRAY;
            float radius = is_stop ? 8.0f : 5.0f;

            DrawCircleV({pos_x, y_pos}, radius, node_color);
            DrawText(outpost.name.c_str(), pos_x - 35, y_pos - 25, 14, LIGHTGRAY);

            if (is_stop) { DrawText("REABASTECER", pos_x - 40, y_pos + 15, 12, YELLOW); }
        }
    }
}