#include "render/window.hpp"
#include "core/probe.hpp"
#include <raylib.h>
#include <vector>

int main() {
    astranav::render::Window window(1280, 720, "astranav-2D");

    astranav::core::Probe voyager("Voyager Calisto", 50.0, 300.0);

    std::vector<astranav::algorithms::Instrument> instruments = {
        {"Espectrômetro", 20.0, 100.0},
        {"Câmera 4K", 30.0, 120.0},
        {"Sensor de Radiação", 10.0, 60.0}
    };
    voyager.load_cargo(instruments);

    std::vector<astranav::algorithms::Outpost> route = {
        {"Estação Alpha", 200.0},
        {"Estação Beta", 450.0},
        {"Estação Gama", 700.0}
    };
    voyager.calculate_flight_plan(800.0, route);

    while(!window.should_close()) {
        /* UPDATE */
        float dt = GetFrameTime();
        voyager.update_simulation(dt, 800.0);

        /* DRAW */
        window.begin_drawing();
        window.clear_background(Color{ 10, 15, 25, 255 });
        
        window.draw_cargo_hud(voyager.get_cargo_manifest(), 50, 50);
        window.draw_flight_plan(voyager.get_flight_plan(), route, 800.0, 360.0f);

        /* ANIMATION */
        window.draw_probe_sprite(voyager.get_position(), voyager.get_fuel(), 800.0, 360.0f);

        window.end_drawing();
    }

    return 0;
}