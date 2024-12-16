// ------------------------------------------------------------------------------------------------
// Title: Car fleet.
// Category: Medium.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// There are n cars traveling to the same destination on a one-lane highway.
//
// You are given two arrays of integers position and speed, both of length n.
//
// - position[i] is the position of the ith car (in miles)
// - speed[i] is the speed of the ith car (in miles per hour)
//
// The destination is at position target miles.
//
// A car can not pass another car ahead of it. It can only catch up to another car and then drive
// at the same speed as the car ahead of it.
//
// A car fleet is a non-empty set of cars driving at the same position and same speed. A single car
// is also considered a car fleet.
//
// If a car catches up to a car fleet the moment the fleet reaches the destination, then the car is
// considered to be part of the fleet.
//
// Return the number of different car fleets that will arrive at the destination.
// ------------------------------------------------------------------------------------------------

#include <algorithm>
#include <common.hpp>
#include <limits>
#include <vector>

struct CarInfo {
    uint32_t position;
    uint32_t velocity;

    // We use a reverse ordering in order to have the cars sorted from:
    //
    //                nearest to target ---> farthest to target
    //
    bool operator<(CarInfo const& other) {
        return (this->position > other.position);
    }
};

// Cars arrive in fleets (think of waves). Each wave accounts for a single distinct fleet. We
// accumulate cars prior to a given one if the time to arrive is less than the time needed for
// the current car to arrive at the target destination.
static uint32_t count_of_car_fleets_arriving(
    uint32_t                     target_position,
    std::vector<uint32_t> const& position,
    std::vector<uint32_t> const& velocity) {
    ptrdiff_t car_count = static_cast<ptrdiff_t>(position.size());

    if (car_count == 0) {
        return 0;
    }

    std::vector<CarInfo> cars;
    cars.resize(car_count);
    for (ptrdiff_t idx = 0; idx < car_count; ++idx) {
        CarInfo& car = cars[idx];
        car.position = position[idx];
        car.velocity = velocity[idx];
    }

    std::sort(cars.begin(), cars.end());

    // At least the first fleet will arrive, and it will lead the max time to arrive for
    // the other fleets.
    uint32_t fleets_arriving_count = 1;
    CarInfo  nearest_car           = cars[0];
    float    fleet_time_to_arrive  = static_cast<float>(target_position - nearest_car.position) / static_cast<float>(nearest_car.velocity);

    for (ptrdiff_t idx = 1; idx < car_count; ++idx) {
        CarInfo car                    = cars[idx];
        float   current_time_to_arrive = static_cast<float>(target_position - car.position) / static_cast<float>(car.velocity);

        // The current car will form a new fleet, as it will arrive post the arrival of the
        // previous fleet (current_time_to_arrive > fleet_time_to_arrive). This bounds the
        // time to arrival of the subsequent cars that are as far or farther than the current
        // car. This ensures that at least one distinct fleet will be created.
        if (current_time_to_arrive > fleet_time_to_arrive) {
            fleet_time_to_arrive = current_time_to_arrive;
            ++fleets_arriving_count;
        }
    }

    return fleets_arriving_count;
}

int main() {
    assert_eq(count_of_car_fleets_arriving(10, {1, 4}, {3, 2}), 1);
    assert_eq(count_of_car_fleets_arriving(10, {4, 1, 0, 7}, {2, 2, 1, 1}), 3);
    report_success();
    return 0;
}
