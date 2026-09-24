#include "core/probe.hpp"

namespace astranav::core {

    Probe::Probe(std::string name, double max_cargo, double max_fuel)
        : name(std::move(name)), max_cargo_capacity(max_cargo),
          max_fuel_autonomy(max_fuel), current_fuel(max_fuel), 
          current_position(0.0), refueling_timer(0.0f), is_refueling(false) {}

    void Probe::load_cargo(const std::vector<algorithms::Instrument>& available_instruments) {
        cargo_manifest = algorithms::optimize_probe_cargo(available_instruments, max_cargo_capacity);
    }

    bool Probe::calculate_flight_plan(double destination_distance, const std::vector<algorithms::Outpost>& route_stations) {
        flight_plan = algorithms::calculate_optimal_route(destination_distance, max_fuel_autonomy, route_stations);
        return flight_plan.is_possible;
    }

    void Probe::update_simulation(float delta_time, double total_distance) {
        if (current_position >= total_distance || !flight_plan.is_possible) return;

        if (is_refueling) {
            refueling_timer += delta_time;

            double refill_rate = max_fuel_autonomy / 1.0;
            current_fuel += refill_rate * delta_time;

            if (current_fuel > max_fuel_autonomy) { current_fuel = max_fuel_autonomy; }

            if (refueling_timer >= 1.5f) {
                is_refueling = false;
                refueling_timer = 0.0f;
                current_position += 0.1;
            }
            return;
        }

        double speed = 120.0;
        double distance_moved = speed * delta_time;

        for (const auto& stop : flight_plan.stops_made) {
            if (current_position < stop.distance_from_earth && (current_position + distance_moved) >= stop.distance_from_earth) {
                current_position = stop.distance_from_earth;
                is_refueling = true;
                return;
            }
        }

        current_position += distance_moved;
        current_fuel -= distance_moved;

        if (current_fuel < 0) current_fuel = 0.0;
        if (current_position > total_distance) current_position = total_distance;
    }

    const algorithms::CargoResult& Probe::get_cargo_manifest() const { return cargo_manifest; }
    const algorithms::RoutePlan& Probe::get_flight_plan() const { return flight_plan; }
    double Probe::get_position() const { return current_position; }
    double Probe::get_fuel() const { return current_fuel; }
    bool Probe::get_is_refueling() const { return is_refueling; }
}
