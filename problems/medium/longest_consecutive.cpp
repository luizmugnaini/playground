// ------------------------------------------------------------------------------------------------
// Title: Longest consecutive.
// Category: Medium.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// Given an array of integers nums, return the length of the longest consecutive sequence of
// elements that can be formed.
//
// A consecutive sequence is a sequence of elements in which each element is exactly 1 greater than
// the previous element. The elements do not have to be consecutive in the original array.
//
// You must write an algorithm that runs in O(n) time.
// ------------------------------------------------------------------------------------------------

#include <algorithm>
#include <type_traits>
#include <unordered_set>
#include <vector>
#include "../common.hpp"

template <typename T>
    requires std::is_integral_v<T>
static size_t longest_consecutive_using_sort(std::vector<T>& values) {
    size_t count = values.size();
    if (count == 0) {
        return 0;
    }

    std::sort(values.begin(), values.end());

    std::vector<T> sequence;
    sequence.reserve(count);

    size_t  max_sequence_count = 0;
    int64_t search_start       = 0;
    while ((search_start >= 0) && (count - search_start > max_sequence_count)) {
        sequence.push_back(values[search_start]);

        int64_t next_search = -1;
        for (size_t idx = search_start + 1; idx < count; ++idx) {
            auto value = values[idx];
            if (value == sequence.back() + 1) {
                sequence.push_back(value);
            } else if (next_search < 0) {
                next_search = idx;  // If next search isn't set, set it.
            }
        }

        max_sequence_count = std::max(max_sequence_count, sequence.size());
        search_start       = next_search;
        sequence.clear();
    }

    return max_sequence_count;
}

template <typename T>
    requires std::is_integral_v<T>
static size_t longest_consecutive_using_unordered_set(std::vector<T> const& values) {
    if (values.empty()) {
        return 0;
    }

    std::unordered_set<T> value_set{values.begin(), values.end()};

    size_t max_sequence_count = 1;

    auto value_set_end = value_set.end();
    for (auto value : values) {
        // If a value has no predecessor in the vector, it must be the start of a sequence.
        bool has_predecessor = (value_set.find(value - 1) != value_set_end);
        if (!has_predecessor) {
            size_t current_sequence_count = 1;

            auto successor_value = value + 1;
            while (value_set.find(successor_value) != value_set_end) {
                ++successor_value;
                ++current_sequence_count;
            }

            max_sequence_count = std::max(max_sequence_count, current_sequence_count);
        }
    }

    return max_sequence_count;
}

int main() {
    // Test 1.
    {
        std::vector values = {2, 20, 4, 10, 3, 4, 5};
        assert_eq(longest_consecutive_using_sort(values), 4);
        assert_eq(longest_consecutive_using_unordered_set(values), 4);
    }

    // Test 2.
    {
        std::vector values = {0, 3, 2, 5, 4, 6, 1, 1};
        assert_eq(longest_consecutive_using_sort(values), 7);
        assert_eq(longest_consecutive_using_unordered_set(values), 7);
    }

    // Test 3.
    {
        std::vector values = {1, 5, 6, 2, -7, -8, 10, -9};
        assert_eq(longest_consecutive_using_sort(values), 3);
        assert_eq(longest_consecutive_using_unordered_set(values), 3);
    }

    report_success();
    return 0;
}
