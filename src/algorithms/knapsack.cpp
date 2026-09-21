#include "algorithms/knapsack.hpp"
#include <algorithm>
#include <ranges>

namespace astranav::algorithms {
    CargoResult optimize_probe_cargo(std::vector<Instrument> instruments, double max_capacity) {
        std::ranges::sort(instruments, [](const Instrument& a, const Instrument& b) {
            return (a.scientific_return / a.weight) > (b.scientific_return / b.weight);
        });

        CargoResult result{0.0, 0.0, {}};
        double current_capacity = 0.0;
        
        for (const auto& inst: instruments) {
            if (current_capacity >= max_capacity) break;

            double remaining_space = max_capacity - current_capacity;

            if (inst.weight <= remaining_space) {
                result.loaded_items.push_back({inst.name, 1.0, inst.weight, inst.scientific_return});
                result.total_weight += inst.weight;
                result.total_scientific_return += inst.scientific_return;
                current_capacity += inst.weight;
            } else {
                double fraction = remaining_space / inst.weight;
                double fractional_return = inst.scientific_return * fraction;

                result.loaded_items.push_back(LoadedItem{inst.name, fraction, remaining_space, fractional_return});
                result.total_weight += remaining_space;
                result.total_scientific_return += fractional_return;
                current_capacity += remaining_space;
                break;
            }
        }
        return result;
    }
}