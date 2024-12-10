// ------------------------------------------------------------------------------------------------
// Title: Sliding window maximum.
// Category: Hard.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// You are given an array of integers nums and an integer k. There is a sliding window of size k
// that starts at the left edge of the array. The window slides one position to the right until it
// reaches the right edge of the array.
//
// Return a list that contains the maximum element in the window at each step.
// ------------------------------------------------------------------------------------------------

#include <algorithm>
#include <common.hpp>
#include <vector>

static inline int32_t find_window_max_value(auto window_start, auto window_end) {
    int32_t maxv = *window_start;
    for (auto it = window_start + 1; it < window_end; ++it) {
        maxv = std::max(maxv, *it);
    }
    return maxv;
}

static std::vector<int32_t> max_sliding_window(std::vector<int32_t> const& values, size_t window_length) {
    size_t values_count = values.size();
    assert(window_length <= values_count);

    std::vector<int32_t> windows_max_values;
    windows_max_values.reserve(values_count - window_length + 1);

    auto window_start = values.begin();
    auto window_end   = values.begin() + window_length;

    // Scan the first window.
    int32_t window_max = find_window_max_value(window_start, window_end);
    windows_max_values.push_back(window_max);

    // Scan the remaining windows.
    auto values_end = window_start + values_count;
    while (window_end < values_end) {
        ++window_start;
        ++window_end;

        int32_t previous_start_value = *(window_start - 1);
        int32_t new_end_value        = *(window_end - 1);

        if (previous_start_value < window_max) {
            window_max = std::max(window_max, new_end_value);
        } else {
            window_max = find_window_max_value(window_start, window_end);
        }

        windows_max_values.push_back(window_max);
    }

    return windows_max_values;
}

int main() {
    debug::vec_assert_eq(max_sliding_window({9}, 1), {9});
    debug::vec_assert_eq(max_sliding_window({9, 2}, 1), {9, 2});
    debug::vec_assert_eq(max_sliding_window({9, 2}, 2), {9});
    debug::vec_assert_eq(max_sliding_window({1, 2, 3}, 1), {1, 2, 3});
    debug::vec_assert_eq(max_sliding_window({1, 2, 3}, 2), {2, 3});
    debug::vec_assert_eq(max_sliding_window({1, 2, 3}, 3), {3});

    report_success();
    return 0;
}
