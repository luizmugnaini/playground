// ------------------------------------------------------------------------------------------------
// Title: Two sum.
// Category: Easy.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// Given an array of integers nums and an integer target, return the indices i and j such that
// nums[i] + nums[j] == target and i != j.
//
// Return the answer with the smaller index first.
// ------------------------------------------------------------------------------------------------

#include <array>
#include <utility>
#include <vector>
#include "../common.hpp"

static std::pair<std::array<size_t, 2>, bool> two_sum(
    std::vector<int> const& values,
    int                     target_sum) noexcept {
    std::array<size_t, 2> index_pair;
    bool                  has_two_sum = false;

    size_t count = values.size();
    for (size_t start = 0; start < count; ++start) {
        for (size_t next = start + 1; next < count; ++next) {
            if (values[start] + values[next] == target_sum) {
                index_pair  = {start, next};
                has_two_sum = true;
                goto finish_two_sum;
            }
        }
    }

finish_two_sum:
    return std::make_pair(index_pair, has_two_sum);
}

int main() {
    std::vector<int> values = {3, 4, 5, 6};

    auto [indices_7_sum, has_7_sum] = two_sum(values, 7);
    assert(has_7_sum);
    assert_eq(indices_7_sum[0], 0);
    assert_eq(indices_7_sum[1], 1);

    auto [indices_9_sum, has_9_sum] = two_sum(values, 9);
    assert(has_9_sum);
    assert_eq(indices_9_sum[0], 0);
    assert_eq(indices_9_sum[1], 3);

    auto [indices_11_sum, has_11_sum] = two_sum(values, 11);
    assert(has_11_sum);
    assert_eq(indices_11_sum[0], 2);
    assert_eq(indices_11_sum[1], 3);

    auto [indices_44_sum, has_44_sum] = two_sum(values, 44);
    assert(!has_44_sum);

    auto [indices_5_sum, has_5_sum] = two_sum(values, 5);
    assert(!has_5_sum);

    report_success();
    return 0;
}
