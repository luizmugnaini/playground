// ------------------------------------------------------------------------------------------------
// Title: Trapping rain water.
// Category: Hard.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// You are given an array non-negative integer heights which represent an elevation map. Each
// value height[i] represents the height of a bar, which has a width of 1.
//
// Return the maximum area of water that can be trapped between the bars.
// ------------------------------------------------------------------------------------------------

#include <common.hpp>
#include <vector>

uint32_t trap_rain_water(std::vector<uint32_t> const& bar_heights) {
    size_t bar_count = bar_heights.size();
    if (bar_count < 3) {
        return 0;
    }

    uint32_t water_accumulator = 0;
    size_t   left              = 0;
    size_t   right             = bar_count - 1;
    uint32_t left_max_height   = bar_heights[left];
    uint32_t right_max_height  = bar_heights[right];

    while (left < right) {
        if (left_max_height >= right_max_height) {
            --right;
            uint32_t current_right_height = bar_heights[right];
            right_max_height              = max_value(right_max_height, current_right_height);

            water_accumulator += right_max_height - current_right_height;
        } else {
            ++left;
            uint32_t current_left_height = bar_heights[left];
            left_max_height              = max_value(left_max_height, current_left_height);

            water_accumulator += left_max_height - current_left_height;
        }
    }

    return water_accumulator;
}

int main() {
    assert_eq(trap_rain_water({0, 2, 0, 3, 1, 0, 1, 3, 2, 1}), 9u);
    assert_eq(trap_rain_water({0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1}), 6u);
    assert_eq(trap_rain_water({0, 0, 0, 5, 0, 0, 0, 0, 0, 0}), 0u);
    assert_eq(trap_rain_water({1, 1, 1, 1, 1, 1, 1, 1, 1, 1}), 0u);
    assert_eq(trap_rain_water({1, 0, 0, 0, 0, 0, 0, 0, 0, 0}), 0u);
    assert_eq(trap_rain_water({0, 0, 0, 0, 0, 0, 0, 0, 0, 1}), 0u);
    assert_eq(trap_rain_water({1, 2, 0, 0, 0, 0, 0, 0, 0, 0}), 0u);
    assert_eq(trap_rain_water({0, 0, 0, 0, 0, 0, 0, 0, 2, 1}), 0u);
    assert_eq(trap_rain_water({2, 1, 0, 0, 0, 0, 0, 0, 0, 0}), 0u);
    assert_eq(trap_rain_water({0, 0, 0, 0, 0, 0, 0, 0, 1, 2}), 0u);

    report_success();
    return 0;
}
