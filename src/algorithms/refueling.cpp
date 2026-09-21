#include "algorithms/refueling.hpp"

namespace astranav::algorithms {
    RoutePlan calculate_optimal_route(double total_distance, double max_autonomy, const std::vector<Outpost>& outposts) {
        RoutePlan plan{true, 0, {}};
        double current_position = 0.0;
        size_t current_outpost_idx = 0;
        
        while (current_position + max_autonomy < total_distance) {
            int best_outpost_idx = -1;
            
            while (current_outpost_idx < outposts.size() && 
                   outposts[current_outpost_idx].distance_from_earth <= current_position + max_autonomy) {
                best_outpost_idx = current_outpost_idx;
                current_outpost_idx++;
            }
            
            if (best_outpost_idx == -1 || outposts[best_outpost_idx].distance_from_earth <= current_position) {
                return {false, 0, {}}; 
            }
            
            current_position = outposts[best_outpost_idx].distance_from_earth;
            plan.stops_made.push_back(outposts[best_outpost_idx]);
            plan.minimum_stops++;
        }
        return plan;
    }
}