#pragma once
#include <string>
#include <vector>
#include "algorithms/knapsack.hpp"
#include "algorithms/refueling.hpp"

namespace astranav::core {
    
    class Probe {
    private:
        std::string name;
        double max_cargo_capacity;
        double max_fuel_autonomy;
        double current_fuel;
        double current_position;

        algorithms::CargoResult cargo_manifest;
        algorithms::RoutePlan flight_plan;
    
    public:
        Probe(std::string name, double max_cargo, double max_fuel);

        void load_cargo(const std::vector<algorithms::Instrument>& available_instruments);
        bool calculate_flight_plan(double destination_distance, const std::vector<algorithms::Outpost>& route_stations);

        const algorithms::CargoResult& get_cargo_manifest() const;
        const algorithms::RoutePlan& get_flight_plan() const;
        double get_position() const;
        double get_fuel() const;
    };
}