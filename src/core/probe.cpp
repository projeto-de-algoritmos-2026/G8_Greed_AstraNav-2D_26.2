#include "core/probe.hpp"

namespace astranav::core {

    Probe::Probe(std::string name, double max_cargo, double max_fuel)
        : name(std::move(name)), max_cargo_capacity(max_cargo),
          max_fuel_autonomy(max_fuel), current_fuel(max_fuel), current_position(0.0) {}

    void Probe::load_cargo(const std::vector<algorithms::Instrument>& available_instruments) {
        cargo_manifest = algorithms::optimize_probe_cargo(available_instruments, max_cargo_capacity);
    }

    bool Probe::calculate_flight_plan(double destination_distance, const std::vector<algorithms::Outpost>& route_stations) {
        flight_plan = algorithms::calculate_optimal_route(destination_distance, max_fuel_autonomy, route_stations);
        return flight_plan.is_possible;
    }

    const algorithms::CargoResult& Probe::get_cargo_manifest() const { return cargo_manifest; }
    const algorithms::RoutePlan& Probe::get_flight_plan() const { return flight_plan; }
    double Probe::get_position() const { return current_position; }
    double Probe::get_fuel() const { return current_fuel; }
}
