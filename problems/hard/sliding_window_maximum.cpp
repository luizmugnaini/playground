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

#include <common.hpp>
#include <vector>

// @NOTE: I hate using the modulo operator here as it is too slow for my taste. But whatever, this is only a
//        worthless exercise.
static std::vector<int32_t> max_in_sliding_windows(std::vector<int32_t> const& values, size_t window_length) {
    size_t values_count = values.size();
    size_t window_count = values_count - window_length + 1;

    std::vector<int32_t> window_max_to_the_left;
    {
        window_max_to_the_left.resize(values_count, 0);

        int32_t current_max_left  = values[0];
        window_max_to_the_left[0] = values[0];

        for (size_t left_idx = 1; left_idx < values_count; ++left_idx) {
            current_max_left = (left_idx % window_length == 0)
                                   ? values[left_idx]
                                   : max_value(current_max_left, values[left_idx]);

            window_max_to_the_left[left_idx] = current_max_left;
        }
    }

    std::vector<int32_t> window_max_to_the_right;
    {
        window_max_to_the_right.resize(values_count, 0);

        int32_t current_max_right                 = values[values_count - 1];
        window_max_to_the_right[values_count - 1] = current_max_right;

        for (ptrdiff_t right_idx = values_count - 2; right_idx >= 0; --right_idx) {
            current_max_right = (right_idx % window_length == 0)
                                    ? values[right_idx]
                                    : max_value(current_max_right, values[right_idx]);

            window_max_to_the_right[right_idx] = current_max_right;
        }
    }

    std::vector<int32_t> window_max_values;
    {
        window_max_values.reserve(window_count);

        ptrdiff_t last_window_start = values_count - window_length;
        for (ptrdiff_t window_start = 0; window_start <= last_window_start; ++window_start) {
            window_max_values.push_back(max_value(
                window_max_to_the_right[window_start],
                window_max_to_the_left[window_start + window_length - 1]));
        }
    }

    return window_max_values;
}

int main() {
    debug::vec_assert_eq(max_in_sliding_windows({9}, 1), {9});
    debug::vec_assert_eq(max_in_sliding_windows({9, 2}, 1), {9, 2});
    debug::vec_assert_eq(max_in_sliding_windows({9, 2}, 2), {9});
    debug::vec_assert_eq(max_in_sliding_windows({1, 2, 3}, 1), {1, 2, 3});
    debug::vec_assert_eq(max_in_sliding_windows({1, 2, 3}, 2), {2, 3});
    debug::vec_assert_eq(max_in_sliding_windows({1, 2, 3}, 3), {3});
    debug::vec_assert_eq(max_in_sliding_windows({1, 3, -1, -3, 5, 3, 6, 7}, 3), {3, 3, 5, 5, 6, 7});

    report_success();
    return 0;
}
