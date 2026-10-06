#include "render/window.hpp"
#include <algorithm>
#include <cmath>
#include <vector>

namespace astranav::render {
    Window::Window(int width, int height, const std::string& title) {
        InitWindow(width, height, title.c_str());
        SetTargetFPS(60);

        // Gera 250 estrelas com tamanhos, opacidades e profundidades variadas
        for (int i = 0; i < 250; i++) {
            Star s;
            s.x = (float)GetRandomValue(0, width);
            s.y = (float)GetRandomValue(0, height);
            s.radius = (float)GetRandomValue(1, 20) / 10.0f; // Tamanhos entre 0.1 e 2.0

            // Estrelas maiores (mais perto) recebem um fator de deslocamento maior
            s.parallax_factor = s.radius * 0.5f;
            s.alpha = (unsigned char)GetRandomValue(80, 255);
            stars.push_back(s);
        }
    }

    Window::~Window(){
        CloseWindow();
    }

    void Window::draw_stars(double probe_position) const {
        float screen_width = (float)GetScreenWidth();

        // Pega o tempo contínuo de execução do jogo em segundos
        float current_time = (float)GetTime();

        for (const auto& star : stars) {
            // Define uma velocidade base para o universo continuar a mover-se sozinho
            float drift_speed = 25.0f;

            // O deslocamento agora é a soma do tempo contínuo e da posição da sonda
            float offset = ((float)probe_position + (current_time * drift_speed)) * star.parallax_factor;

            // Envolve a coordenada X (wrapping)
            float draw_x = fmodf(star.x - offset, screen_width);
            if (draw_x < 0) {
                draw_x += screen_width;
            }

            Color star_color = { 255, 255, 255, star.alpha };
            DrawCircleV({draw_x, star.y}, star.radius, star_color);
        }
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

        float padding = 15.0f;
        float item_width = sidebar_width - (padding * 2.0f); // 360 - 30 = 330px de largura útil

        // 2. Títulos da Sidebar
        DrawText("PAINEL DE CONTROLE", padding, y, 20, RAYWHITE);
        DrawText("MANIFESTO DE CARGA", padding, y + 35, 14, SKYBLUE);

        float current_y = y + 70;

        // 3. Listagem de itens de carga
        for (const auto& item : cargo.loaded_items) {
            Color item_color;
            if (item.fraction_taken == 0.0) {
                item_color = DARKGRAY; 
            } else if (item.fraction_taken < 1.0) {
                item_color = ORANGE;   
            } else {
                item_color = LIME;     
            }
            
            DrawRectangle(x + 15, current_y, 290, 35, Fade(item_color, 0.3f));
            DrawRectangleLines(x + 15, current_y, 290, 35, item_color);
            
            const char* text = TextFormat("%s (%.0f%%)", item.name.c_str(), item.fraction_taken * 100);
            DrawText(text, x + 25, current_y + 8, 16, WHITE);
            
            current_y += 50;
        }

        // 4. Estatísticas de Carga
        DrawText(TextFormat("Peso Usado: %.1f kg", cargo.total_weight), padding, current_y + 10, 16, LIGHTGRAY);
        DrawText(TextFormat("Retorno Científico: %.1f", cargo.total_scientific_return), padding, current_y + 35, 16, GREEN);

        // 5. Seção de Controles da Simulação
        float controls_y = screen_height - 140.0f;
        DrawLineEx({padding, controls_y}, {sidebar_width - padding, controls_y}, 1.0f, Fade(LIGHTGRAY, 0.3f));
        DrawText("CONTROLES DE SIMULAÇÃO", padding, controls_y + 15, 14, GRAY);

        float button_width = (item_width - 10.0f) / 2.0f; // Divide o espaço em duas colunas simétricas

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

        int index = 0;
        for (const auto& outpost : all_outposts) {
            float pos_x = start_x + (outpost.distance_from_earth / total_distance) * width;

            bool is_stop = std::ranges::any_of(plan.stops_made,
                [&](const auto& stop) { return stop.name == outpost.name; });

            if (is_stop) {
                DrawCircle((int)pos_x, (int)y_pos, 12.0f, Fade(YELLOW, 0.3f));

                DrawCircleGradient((int)pos_x, (int)y_pos, 8.0f, YELLOW, ORANGE);
            } else {
                DrawCircle((int)pos_x, (int)y_pos, 7.0f, Fade(DARKGRAY, 0.3f));
                DrawCircleGradient((int)pos_x, (int)y_pos, 5.0f, GRAY, DARKGRAY);
            }

            float text_y_offset = (index % 2 == 0) ? -32.0f : 18.0f;
            DrawText(outpost.name.c_str(), pos_x - 30, y_pos + text_y_offset, 14, LIGHTGRAY);

            if (is_stop) { DrawText("REABASTECER", pos_x - 40, y_pos + 35, 12, YELLOW); }
            index++;
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

    void Window::draw_progress_bar(double current_pos, double total_distance, float x, float y, float width, float height) const {
        if (total_distance <= 0.0) return;
        
        float progress = static_cast<float>(current_pos / total_distance);
        if (progress > 1.0f) progress = 1.0f;
        if (progress < 0.0f) progress = 0.0f;

        DrawRectangle((int)x, (int)y, (int)width, (int)height, Fade(DARKGRAY, 0.6f));
        DrawRectangleLines((int)x, (int)y, (int)width, (int)height, LIGHTGRAY);

        float filled_width = width * progress;
        DrawRectangle((int)x, (int)y, (int)filled_width, (int)height, LIME);

        const char* text = TextFormat("Progresso: %.1f / %.1f km (%.1f%%)", current_pos, total_distance, progress * 100.0f);
        DrawText(text, (int)x + (int)(width / 2) - MeasureText(text, 12) / 2, (int)y - 18, 12, RAYWHITE);
    }
}