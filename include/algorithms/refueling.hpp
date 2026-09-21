#pragma once
#include <vector>
#include <string>

namespace astranav::algorithms {
    struct Outpost {
        std::string name;
        double distance_from_earth;
    };

    struct RoutePlan {
        bool is_possible;
        int minimum_stops;
        std::vector<Outpost> stops_made;
    };

    RoutePlan calculate_optimal_route(double total_distance, double max_autonomy, const std::vector<Outpost>& outposts);
}