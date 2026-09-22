#pragma once
#include <string>
#include <raylib.h>
#include "algorithms/knapsack.hpp"

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
    };
}