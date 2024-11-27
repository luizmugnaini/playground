// ------------------------------------------------------------------------------------------------
// Title: Two sum with non-decreasing array.
// Category: Easy.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
// Given an array of integers numbers that is sorted in non-decreasing order.
//
// Return the indices (1-indexed) of two numbers, [index1, index2], such that they add up to a
// given target number target and index1 < index2. Note that index1 and index2 cannot be equal,
// therefore you may not use the same element twice.
//
// There will always be exactly one valid solution.
//
// Your solution must use O(1) additional space.
// ------------------------------------------------------------------------------------------------

#include <algorithm>
#include <utility>
#include <vector>
#include "../common.hpp"

template <typename T>
std::pair<size_t, size_t> two_sum_indices(std::vector<T> const& non_decreasing_values, T target_sum) {
    size_t count = non_decreasing_values.size();

    T max_value = *std::max_element(non_decreasing_values.begin(), non_decreasing_values.end());

    size_t first_idx  = 0;
    size_t second_idx = 0;

    for (size_t search_first = 0; search_first < count; ++search_first) {
        T first_value = non_decreasing_values[search_first];

        if (first_value + max_value < target_sum) {
            continue;
        }

        for (size_t search_second = search_first + 1; search_second < count; ++search_second) {
            T second_value = non_decreasing_values[search_second];
            T sum          = first_value + second_value;

            if (sum == target_sum) {
                first_idx  = search_first;
                second_idx = search_second;
                goto finish_two_sum_indices;
            } else if (sum > target_sum) {
                break;  // Any other succeeding value will give us a bigger sum.
            }
        }
    }

finish_two_sum_indices:
    // Since we'll use a 1-indexed thingy, we add 1 to both indices.
    return std::make_pair(first_idx + 1, second_idx + 1);
}

int main() {
    // Test 1.
    {
        auto [first, second] = two_sum_indices({10, 6, 3, 4}, 16);
        assert_eq(first, 1);   // Value: 10.
        assert_eq(second, 2);  // Value: 6.
    }

    // Test 2.
    {
        auto [first, second] = two_sum_indices({0, -7, 1, 9, 3}, 2);
        assert_eq(first, 2);   // Value: -7.
        assert_eq(second, 4);  // Value: 9.
    }

    report_success();
    return 0;
}
