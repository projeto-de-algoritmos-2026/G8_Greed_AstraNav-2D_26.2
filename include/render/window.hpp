#pragma once
#include <string>
#include <raylib.h>
#include "algorithms/knapsack.hpp"
#include "algorithms/refueling.hpp"
#include <vector>

namespace astranav::render {

    struct Star {
        float x, y;
        float radius;
        float parallax_factor;
        unsigned char alpha;
    };
    class Window {
    private:
        std::vector<Star> stars;
    public:
        Window(int width, int height, const std::string& title);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        void draw_alert_box(const std::string& message) const;
        bool should_close() const;
        void begin_drawing() const;
        void end_drawing() const;
        void clear_background(Color color) const;
        void draw_stars(double probe_position) const;
        void draw_cargo_hud(const algorithms::CargoResult& cargo, float x, float y) const;
        void draw_flight_plan(const algorithms::RoutePlan& plan, const std::vector<algorithms::Outpost>& all_outposts, double total_distance, float y_pos) const;
        void draw_probe_sprite(double current_pos, double current_fuel, double total_distance, float y_pos) const;
    };
}