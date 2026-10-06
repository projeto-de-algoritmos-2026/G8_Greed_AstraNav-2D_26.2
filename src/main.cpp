#include "render/window.hpp"
#include "core/probe.hpp"
#include <raylib.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>

int main() {
    astranav::render::Window window(1280, 720, "astranav-2D");

    // Loop externo global: Permite voltar ao menu quantas vezes quiser até fechar a janela
    while (!window.should_close()) {

        // --- ESTADO 1: TELA DE SETUP INTERATIVA ---
        std::string input_dist = "800";
        std::string input_fuel = "300";
        
        std::string input_item_name = "Sensor X";
        std::string input_item_weight = "15";
        std::string input_item_return = "80";

        int active_field = 0; 
        bool setup_complete = false;
        std::string error_message = "";

        std::vector<astranav::algorithms::Instrument> instruments = {
            {"Espectrometro", 20.0, 100.0}, 
            {"Camera 4K", 30.0, 120.0},     
            {"Sensor Rad", 10.0, 60.0}
        };

        while (!window.should_close() && !setup_complete) {
            int key = GetCharPressed();
            while (key > 0) {
                if (active_field == 2) {
                    if ((key >= 32 && key <= 126) && input_item_name.length() < 15) {
                        input_item_name += (char)key;
                    }
                } else {
                    if (key >= 48 && key <= 57) {
                        if (active_field == 0) input_dist += (char)key;
                        else if (active_field == 1) input_fuel += (char)key;
                        else if (active_field == 3) input_item_weight += (char)key;
                        else if (active_field == 4) input_item_return += (char)key;
                    }
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (active_field == 0 && !input_dist.empty()) input_dist.pop_back();
                else if (active_field == 1 && !input_fuel.empty()) input_fuel.pop_back();
                else if (active_field == 2 && !input_item_name.empty()) input_item_name.pop_back();
                else if (active_field == 3 && !input_item_weight.empty()) input_item_weight.pop_back();
                else if (active_field == 4 && !input_item_return.empty()) input_item_return.pop_back();
            }

            if (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_DOWN)) {
                active_field = (active_field + 1) % 5;
            } else if (IsKeyPressed(KEY_UP)) {
                active_field = (active_field - 1 + 5) % 5;
            }

            if (IsKeyPressed(KEY_F2)) {
                if (instruments.size() >= 6) {
                    error_message = "Erro: Limite maximo de 6 itens atingido!";
                } else if (!input_item_name.empty() && !input_item_weight.empty() && !input_item_return.empty()) {
                    double w = std::stod(input_item_weight);
                    double r = std::stod(input_item_return);

                    if (w <= 0 || w > 50.0) {
                        error_message = "Erro: O peso deve ser entre 1 e 50 kg!";
                    } else if (r <= 0 || r > 1000.0) {
                        error_message = "Erro: O retorno deve ser entre 1 e 1000!";
                    } else {
                        instruments.push_back({input_item_name, w, r});
                        error_message = "Item adicionado com sucesso!";
                        input_item_name = "Novo Item";
                        input_item_weight = "10";
                        input_item_return = "50";
                    }
                }
            }

            if (IsKeyPressed(KEY_DELETE)) {
                if (instruments.size() > 1) {
                    instruments.pop_back();
                    error_message = "Ultimo item removido!";
                } else {
                    error_message = "Erro: Mantenha pelo menos 1 item na lista!";
                }
            }

            if (IsKeyPressed(KEY_ENTER) && !input_dist.empty() && !input_fuel.empty()) {
                setup_complete = true;
            }

            window.begin_drawing();
            window.clear_background(Color{ 10, 15, 25, 255 }); 
            
            DrawText("ASTRANAV-2D - SETUP DA MISSAO", 410, 30, 24, RAYWHITE);
            
            DrawText("Distancia Total (km):", 100, 80, 16, GRAY);
            DrawRectangle(100, 105, 300, 35, active_field == 0 ? DARKGRAY : BLACK);
            DrawRectangleLines(100, 105, 300, 35, active_field == 0 ? GREEN : LIGHTGRAY);
            DrawText(input_dist.c_str(), 115, 113, 18, WHITE);

            DrawText("Autonomia Sonda (km):", 100, 155, 16, GRAY);
            DrawRectangle(100, 180, 300, 35, active_field == 1 ? DARKGRAY : BLACK);
            DrawRectangleLines(100, 180, 300, 35, active_field == 1 ? GREEN : LIGHTGRAY);
            DrawText(input_fuel.c_str(), 115, 188, 18, WHITE);

            DrawText("Criar Instrumento Cientifico (Max: 6 itens):", 100, 235, 18, RAYWHITE);
            
            DrawText("Nome:", 100, 265, 14, GRAY);
            DrawRectangle(100, 285, 180, 30, active_field == 2 ? DARKGRAY : BLACK);
            DrawRectangleLines(100, 285, 180, 30, active_field == 2 ? GREEN : LIGHTGRAY);
            DrawText(input_item_name.c_str(), 110, 291, 16, WHITE);

            DrawText("Peso (kg):", 290, 265, 14, GRAY);
            DrawRectangle(290, 285, 100, 30, active_field == 3 ? DARKGRAY : BLACK);
            DrawRectangleLines(290, 285, 100, 30, active_field == 3 ? GREEN : LIGHTGRAY);
            DrawText(input_item_weight.c_str(), 300, 291, 16, WHITE);

            DrawText("Retorno:", 405, 265, 14, GRAY);
            DrawRectangle(405, 285, 100, 30, active_field == 4 ? DARKGRAY : BLACK);
            DrawRectangleLines(405, 285, 100, 30, active_field == 4 ? GREEN : LIGHTGRAY);
            DrawText(input_item_return.c_str(), 415, 291, 16, WHITE);

            DrawText("[ Pressione 'F2' para adicionar | Pressione 'DELETE' para remover ]", 100, 325, 14, YELLOW);

            if (!error_message.empty()) {
                DrawText(error_message.c_str(), 100, 350, 14, error_message[0] == 'E' ? RED : LIME);
            }

            DrawText("Itens na Lista para a Mochila:", 100, 385, 16, GRAY);
            int list_y = 410;
            for (const auto& inst : instruments) {
                DrawText(TextFormat("- %s (Peso: %.1f kg | Retorno: %.1f)", inst.name.c_str(), inst.weight, inst.scientific_return), 100, list_y, 14, LIGHTGRAY);
                list_y += 20;
            }

            DrawText("Pressione TAB para alternar campos | ENTER para iniciar simulacao", 100, 560, 16, DARKGRAY);

            window.end_drawing();
        }

        if (window.should_close()) break;

        double destination_distance = std::stod(input_dist);
        double custom_fuel = std::stod(input_fuel);

        // --- TRANSIÇÃO: GERAÇÃO PROCEDURAL DA ROTA ADAPTADA À AUTONOMIA ---
        std::random_device rd;
        std::mt19937 gen(rd()); 
        std::uniform_int_distribution<> num_dist(3, 6);
        int num_stations = num_dist(gen);

        if (destination_distance < 500.0) destination_distance = 500.0; 

        std::vector<double> random_distances;
        double max_step = custom_fuel * 0.8; 
        if (max_step < 80.0) max_step = 80.0;

        double current_pos = 0.0;
        while (current_pos < destination_distance - 100.0) {
            std::uniform_real_distribution<> step_dist(60.0, max_step);
            current_pos += step_dist(gen);
            
            if (current_pos < destination_distance - 50.0) {
                random_distances.push_back(current_pos);
            } else {
                break;
            }
        }

        std::ranges::sort(random_distances);

        std::vector<astranav::algorithms::Outpost> route;
        for (size_t i = 0; i < random_distances.size(); ++i) {
            route.push_back({"Estacao " + std::to_string(i + 1), random_distances[i]});
        }

        astranav::core::Probe voyager("Voyager Calisto", 50.0, custom_fuel); 
        voyager.load_cargo(instruments); 
        
        bool is_route_possible = voyager.calculate_flight_plan(destination_distance, route);

        // --- ESTADO 2: SIMULAÇÃO PRINCIPAL ---
        bool return_to_menu = false;
        bool is_paused = false;
        float simulation_speed = 1.0f;

        Rectangle btn_play_rect = { 45, 630, 130, 35 };
        Rectangle btn_speed_rect = { 185, 630, 140, 35 };

        while (!window.should_close() && !return_to_menu) {
            Vector2 mouse_pos = GetMousePosition();

            // Atalho BACKSPACE para voltar ao menu a qualquer momento
            if (IsKeyPressed(KEY_BACKSPACE)) {
                return_to_menu = true;
                break;
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mouse_pos, btn_play_rect)) {
                    is_paused = !is_paused;
                }
                if (CheckCollisionPointRec(mouse_pos, btn_speed_rect)) {
                    if (simulation_speed == 1.0f) simulation_speed = 2.0f;
                    else if (simulation_speed == 2.0f) simulation_speed = 4.0f;
                    else simulation_speed = 1.0f;
                }
            }

            float raw_dt = GetFrameTime();
            float dt = is_paused ? 0.0f : (raw_dt * simulation_speed);

            if (is_route_possible) {
                voyager.update_simulation(dt, destination_distance);
            }

            window.begin_drawing();
            window.clear_background(Color{ 10, 15, 25, 255 }); 
            
            if (is_route_possible) {
                window.draw_stars(voyager.get_position());
                window.draw_cargo_hud(voyager.get_cargo_manifest(), 50, 50);
                window.draw_flight_plan(voyager.get_flight_plan(), route, destination_distance, 360.0f);
                window.draw_progress_bar(voyager.get_position(), destination_distance, 450.0f, 550.0f, 700.0f, 20.0f);
                window.draw_probe_sprite(voyager.get_position(), voyager.get_fuel(), destination_distance, 360.0f);
            } else {
                window.draw_cargo_hud(voyager.get_cargo_manifest(), 50, 50);
                window.draw_flight_plan(voyager.get_flight_plan(), route, destination_distance, 360.0f);
                window.draw_alert_box("Alerta: Autonomia insuficiente para alcancar o destino!");
            }

            // Botões e dica de atalho BACKSPACE na tela
            DrawRectangleRec(btn_play_rect, CheckCollisionPointRec(mouse_pos, btn_play_rect) ? DARKGRAY : BLACK);
            DrawRectangleLines((int)btn_play_rect.x, (int)btn_play_rect.y, (int)btn_play_rect.width, (int)btn_play_rect.height, is_paused ? YELLOW : LIME);
            DrawText(is_paused ? "[ PLAY ]" : "[ PAUSE ]", btn_play_rect.x + 28, btn_play_rect.y + 10, 14, RAYWHITE);

            DrawRectangleRec(btn_speed_rect, CheckCollisionPointRec(mouse_pos, btn_speed_rect) ? DARKGRAY : BLACK);
            DrawRectangleLines((int)btn_speed_rect.x, (int)btn_speed_rect.y, (int)btn_speed_rect.width, (int)btn_speed_rect.height, LIGHTGRAY);
            DrawText(TextFormat("[ VEL: %.0fX ]", simulation_speed), btn_speed_rect.x + 22, btn_speed_rect.y + 10, 14, RAYWHITE);

            DrawText("[ Pressione BACKSPACE para voltar ao Menu ]", 950, 20, 14, LIGHTGRAY);

            window.end_drawing();
        }
    }
    
    return 0;
}