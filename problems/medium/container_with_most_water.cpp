// ------------------------------------------------------------------------------------------------
// Title: Container with most water.
// Category: Medium.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// You are given an integer array heights where heights[i] represents the height of the ithith bar.
//
// You may choose any two bars to form a container. Return the maximum amount of water a container
// can store.
// ------------------------------------------------------------------------------------------------

#include <algorithm>
#include <vector>
#include "../common.hpp"

#define max_value(lhs, rhs) (((lhs) >= (rhs)) ? (lhs) : (rhs))
#define min_value(lhs, rhs) (((lhs) <= (rhs)) ? (lhs) : (rhs))

uint32_t max_water_brute_force(std::vector<uint32_t> const& bar_heights) {
    size_t bar_count = bar_heights.size();

    uint32_t max_water = 0;
    for (size_t left = 0; left < bar_count; ++left) {
        uint32_t left_height = bar_heights[left];
        for (size_t right = bar_count - 1; right > left; --right) {
            max_water = max_value(max_water, min_value(left_height, bar_heights[right]) * static_cast<uint32_t>(right - left));
        }
    }

    return max_water;
}

uint32_t max_water(std::vector<uint32_t> const& bar_heights) {
    uint32_t max_water = 0;
    size_t   left      = 0;
    size_t   right     = bar_heights.size() - 1;
    while (left < right) {
        auto left_height  = bar_heights[left];
        auto right_height = bar_heights[right];

        max_water = max_value(max_water, min_value(left_height, right_height) * static_cast<uint32_t>(right - left));

        if (left_height <= right_height) {
            ++left;
        } else {
            --right;
        }
    }

    return max_water;
}

int main() {
    assert_eq(max_water_brute_force({1, 7, 2, 5, 4, 7, 3, 6}), 36u);
    assert_eq(max_water({1, 7, 2, 5, 4, 7, 3, 6}), 36u);

    assert_eq(max_water_brute_force({2, 2, 2}), 4u);
    assert_eq(max_water({2, 2, 2}), 4u);

    report_success();
    return 0;
}
