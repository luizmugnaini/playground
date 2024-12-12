// ------------------------------------------------------------------------------------------------
// Title: Koko eating bananas.
// Category: Medium.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// You are given an integer array piles where piles[i] is the number of bananas in the ith pile.
// You are also given an integer h, which represents the number of hours you have to eat all the
// bananas.
//
// You may decide your bananas-per-hour eating rate of k. Each hour, you may choose a pile of
// bananas and eats k bananas from that pile. If the pile has less than k bananas, you may finish
// eating the pile but you can not eat from another pile in the same hour.
//
// Return the minimum integer k such that you can eat all the bananas within h hours.
// ------------------------------------------------------------------------------------------------

#include <algorithm>
#include <cmath>
#include <common.hpp>
#include <vector>

uint32_t min_eating_velocity_per_banana_pile_per_hour(std::vector<uint32_t> banana_piles, uint32_t hour_limit) {
    size_t pile_count = banana_piles.size();
    assert((pile_count <= hour_limit) && "Not enough time to eat all banana piles");

    // The lowest velocity is to eat 1 banana per hour.
    uint32_t low_velocity = 1;

    // The "worst" (non-minimum) case would be to eat each pile per hour.
    uint32_t high_velocity = (pile_count != 0) ? *std::max_element(banana_piles.begin(), banana_piles.end()) : 0;

    uint32_t result_velocity = high_velocity;

    while (low_velocity <= high_velocity) {
        uint32_t mid_velocity = (low_velocity + high_velocity) / 2u;

        uint32_t time_to_eat_all = 0;
        for (uint32_t banana_count : banana_piles) {
            time_to_eat_all += static_cast<uint32_t>(std::ceil(static_cast<double>(banana_count) / static_cast<double>(mid_velocity)));
        }

        if (time_to_eat_all <= hour_limit) {
            // Valid, but there's possibly some under-utilized time.
            result_velocity = mid_velocity;
            high_velocity   = mid_velocity - 1u;
        } else {
            // Too slow, we surpassed the time limit.
            low_velocity = mid_velocity + 1u;
        }
    }

    return result_velocity;
}

int main() {
    assert_eq(min_eating_velocity_per_banana_pile_per_hour({}, 3), 0);
    assert_eq(min_eating_velocity_per_banana_pile_per_hour({0, 0, 0, 0, 0}, 9), 0);
    assert_eq(min_eating_velocity_per_banana_pile_per_hour({1, 4, 3, 2}, 9), 2);
    assert_eq(min_eating_velocity_per_banana_pile_per_hour({25, 10, 23, 4}, 4), 25);
    assert_eq(min_eating_velocity_per_banana_pile_per_hour({1, 1, 1, 999999999}, 10), 142857143);
    report_success();
    return 0;
}
