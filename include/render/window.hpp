#pragma once
#include <string>
#include <raylib.h>
#include "algorithms/knapsack.hpp"
#include "algorithms/refueling.hpp"

namespace astranav::render {

    class Window {
    public:
        Window(int width, int height, const std::string& title);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        bool should_close() const;
        void begin_drawing() const;
        void end_drawing() const;
        void clear_background(Color color) const;

        void draw_cargo_hud(const algorithms::CargoResult& cargo, float x, float y) const;
        void draw_flight_plan(const algorithms::RoutePlan& plan, const std::vector<algorithms::Outpost>& all_outposts, double total_distance, float y_pos) const;
    };
}