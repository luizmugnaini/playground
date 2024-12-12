// ------------------------------------------------------------------------------------------------
// Title: Binary search.
// Category: Easy.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// You are given an array of distinct integers nums, sorted in ascending order, and an integer
// target.
//
// Implement a function to search for target within nums. If it exists, then return its index,
// otherwise, return -1.
//
// Your solution must run in O(log(n)) time.
// ------------------------------------------------------------------------------------------------

#include <common.hpp>
#include <type_traits>
#include <vector>

template <typename T>
    requires std::is_integral_v<T>
ptrdiff_t binary_search_range(std::vector<T> const& values, T target, ptrdiff_t low, ptrdiff_t high) {
    if (high < low) {
        return -1;
    }

    ptrdiff_t mid       = (high + low) / 2;
    T         mid_value = values[mid];

    ptrdiff_t new_low  = low;
    ptrdiff_t new_high = high;
    if (mid_value < target) {
        new_low = mid + 1;
    } else if (mid_value > target) {
        new_high = mid - 1;
    } else {
        return mid;
    }

    return binary_search_range(values, target, new_low, new_high);
}

template <typename T>
    requires std::is_integral_v<T>
ptrdiff_t binary_search(std::vector<T> const& values, T target) {
    return binary_search_range(values, target, 0, static_cast<ptrdiff_t>(values.size()) - 1);
}

int main() {
    assert_eq(binary_search({5}, -5), -1);
    assert_eq(binary_search({-1, 0, 2, 4, 6, 8}, 4), 3);
    assert_eq(binary_search({-1, 0, 2, 4, 6, 8}, 3), -1);
    assert_eq(binary_search({-1, 0, 2, 4, 6, 8}, -1), 0);
    assert_eq(binary_search({-1, 0, 2, 4, 6, 8}, 8), 5);

    report_success();
    return 0;
}
