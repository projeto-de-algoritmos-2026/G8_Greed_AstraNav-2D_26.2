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

    void Window::draw_alert_box(const std::string& message) const {
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(RED, 0.7f));

        int fontSize = 30;
        int textWidth = MeasureText(message.c_str(), fontSize);
        DrawText(message.c_str(), (screenWidth - textWidth) / 2, (screenHeight - fontSize) / 2, fontSize, WHITE);
    }

    bool Window::should_close() const { return WindowShouldClose(); }
    void Window::begin_drawing() const { BeginDrawing(); }
    void Window::end_drawing() const { EndDrawing(); }
    void Window::clear_background(Color color) const { ClearBackground(color); }

    void Window::draw_cargo_hud(const algorithms::CargoResult& cargo, float x, float y) const {
        float sidebar_width = 360.0f;
        float screen_height = (float)GetScreenHeight();

        // 1. Fundo sólido da sidebar e borda
        DrawRectangleRounded(Rectangle{0, 0, sidebar_width, screen_height}, 0.02f, 4, Color{15, 20, 30, 255});
        DrawRectangleRoundedLines(Rectangle{0, 0, sidebar_width, screen_height}, 0.02f, 4, 2.0f, Fade(LIGHTGRAY, 0.2f));

        // Margem interna segura para os elementos da sidebar
        float padding = 15.0f;
        float item_width = sidebar_width - (padding * 2.0f); // 360 - 30 = 330px de largura útil

        // 2. Títulos da Sidebar
        DrawText("PAINEL DE CONTROLO", padding, y, 20, RAYWHITE);
        DrawText("MANIFESTO DE CARGA", padding, y + 35, 14, SKYBLUE);

        float current_y = y + 70;

        // 3. Listagem de itens de carga
        for (const auto& item : cargo.loaded_items) {
            Color item_color = (item.fraction_taken < 1.0) ? ORANGE : LIME;

            DrawRectangleRounded(Rectangle{padding, current_y, item_width, 40}, 0.1f, 4, Fade(item_color, 0.2f));
            DrawRectangleRoundedLines(Rectangle{padding, current_y, item_width, 40}, 0.1f, 4, 1.5f, item_color);

            const char* text = TextFormat("%s, (%.0f%%)", item.name.c_str(), item.fraction_taken * 100);
            DrawText(text, padding + 10, current_y + 10, 16, WHITE);

            current_y += 50;
        }

        // 4. Estatísticas de Carga
        DrawText(TextFormat("Peso Usado: %.1f kg", cargo.total_weight), padding, current_y + 10, 16, LIGHTGRAY);
        DrawText(TextFormat("Retorno Científico: %.1f", cargo.total_scientific_return), padding, current_y + 35, 16, GREEN);

        // 5. Seção de Controlos da Simulação
        float controls_y = screen_height - 140.0f;
        DrawLineEx({padding, controls_y}, {sidebar_width - padding, controls_y}, 1.0f, Fade(LIGHTGRAY, 0.3f));
        DrawText("CONTROLOS DA SIMULAÇÃO", padding, controls_y + 15, 14, GRAY);

        // Botões ajustados para caber perfeitamente dentro dos 360px
        float button_width = (item_width - 10.0f) / 2.0f; // Divide o espaço em duas colunas simétricas

        DrawRectangleRounded(Rectangle{padding, controls_y + 45, button_width, 35}, 0.2f, 4, Fade(DARKGRAY, 0.5f));
        DrawText("[ PLAY / PAUSE ]", padding + 8, controls_y + 55, 11, LIGHTGRAY);

        DrawRectangleRounded(Rectangle{padding + button_width + 10.0f, controls_y + 45, button_width, 35}, 0.2f, 4, Fade(DARKGRAY, 0.5f));
        DrawText("[ VELOCIDADE 1X ]", padding + button_width + 15.0f, controls_y + 55, 11, LIGHTGRAY);
    }

    void Window::draw_flight_plan(const algorithms::RoutePlan& plan, const std::vector<algorithms::Outpost>& all_outposts, double total_distance, float y_pos) const {
        float start_x = 420.0f;
        float end_x = 1200.0f;
        float width = end_x - start_x;

        /* Linha da Trajetoria */
        DrawLineEx({start_x, y_pos}, {end_x, y_pos}, 2.0f, Fade(LIGHTGRAY, 0.4f));
        
        /* Terra */
        // 1. Glow / Atmosfera sutil ao redor da Terra
        DrawCircle((int)start_x, (int)y_pos, 16.0f, Fade(SKYBLUE, 0.3f));

        // 2. Corpo do planeta com gradiente radial (dando efeito esférico/volume)
        DrawCircleGradient((int)start_x, (int)y_pos, 12.0f, SKYBLUE, DARKBLUE);

        // Label
        DrawText("Terra", start_x - 15, y_pos + 20, 18, RAYWHITE);

        /* Destino */
        // 1. Glow / Atmosfera sutil ao redor do Destino
        DrawCircle((int)end_x, (int)y_pos, 22.0f, Fade(PINK, 0.3f));

        // 2. Corpo do planeta com gradiente radial
        DrawCircleGradient((int)end_x, (int)y_pos, 16.0f, MAGENTA, DARKPURPLE);

        // Label
        DrawText("Destino", end_x - 30, y_pos + 25, 18, RAYWHITE);

        for (const auto& outpost : all_outposts) {
            float pos_x = start_x + (outpost.distance_from_earth / total_distance) * width;

            bool is_stop = std::ranges::any_of(plan.stops_made,
                [&](const auto& stop) { return stop.name == outpost.name; });

            if (is_stop) {
                // 1. Halo atmosférico (glow) para a estação de reabastecimento ativa
                DrawCircle((int)pos_x, (int)y_pos, 12.0f, Fade(YELLOW, 0.3f));

                // 2. Gradiente radial volumétrico 3D (amarelo para laranja)
                DrawCircleGradient((int)pos_x, (int)y_pos, 8.0f, YELLOW, ORANGE);
            } else {
                // Estações secundárias/inativas (mais discretas, mas com volume 3D)
                DrawCircle((int)pos_x, (int)y_pos, 7.0f, Fade(DARKGRAY, 0.3f));
                DrawCircleGradient((int)pos_x, (int)y_pos, 5.0f, GRAY, DARKGRAY);
            }

            DrawText(outpost.name.c_str(), pos_x - 35, y_pos - 25, 14, LIGHTGRAY);

            if (is_stop) { DrawText("REABASTECER", pos_x - 40, y_pos + 15, 12, YELLOW); }
        }
    }

    void Window::draw_probe_sprite(double current_pos, double current_fuel, double total_distance, float y_pos) const {
        float start_x = 420.0f;
        float end_x = 1200.0f;
        float width = end_x - start_x;

        float pos_x = start_x + (current_pos / total_distance) * width;

        Vector2 v1 = {pos_x + 15, y_pos};
        Vector2 v2 = {pos_x - 10, y_pos - 15};
        Vector2 v3 = {pos_x - 10, y_pos + 15};

        DrawTriangle(v1, v2, v3, LIME);

        Color fuel_color = (current_fuel > 50.0) ? LIME : RED;
        DrawText(TextFormat("Combustivel: %.1f", current_fuel), pos_x - 40, y_pos - 40, 16, fuel_color);
    }
}