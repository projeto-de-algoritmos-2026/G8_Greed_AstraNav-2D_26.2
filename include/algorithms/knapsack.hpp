#pragma once
#include <string>
#include <vector>

namespace astranav::algorithms {
    struct Instrument {
        std::string name;
        double weight;
        double scientific_return;
    };

    struct LoadedItem {
        std::string name;
        double fraction_taken;
        double weight_taken;
        double return_achieved;
    };

    struct CargoResult {
        double total_weight;
        double total_scientific_return;
        std::vector<LoadedItem> loaded_items;
    };

    CargoResult optimize_probe_cargo(std::vector<Instrument> instruments, double max_capacity);
}