// ------------------------------------------------------------------------------------------------
// Title: Three sum.
// Category: Medium.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// Given an integer array nums, return all the triplets [nums[i], nums[j], nums[k]] where
// nums[i] + nums[j] + nums[k] == 0, and the indices i, j and k are all distinct.
//
// The output should not contain any duplicate triplets. You may return the output and the
// triplets in any order.
// ------------------------------------------------------------------------------------------------

#include <algorithm>
#include <array>
#include <vector>
#include "../common.hpp"

template <typename T>
using Triple = std::array<T, 3>;

template <typename T>
std::vector<Triple<T>> three_sum(std::vector<T> const& values, T target) {
    size_t count = values.size();
    if (count == 0) {
        return {};
    }

    std::vector<Triple<T>> triplets;
    triplets.reserve(count / 3);  // Tries to avoid more allocations...

    T max_value = *std::max_element(values.begin(), values.end());
    T min_value = *std::min_element(values.begin(), values.end());

    for (size_t first = 0; first < count; ++first) {
        T first_value = values[first];

        for (size_t second = first + 1; second < count; ++second) {
            T second_value = values[second];
            T remaining    = target - (first_value + second_value);

            if ((remaining < min_value) || (remaining > max_value)) {
                continue;
            }

            for (size_t third = second + 1; third < count; ++third) {
                T third_value = values[third];
                if (third_value == remaining) {
                    triplets.emplace_back(Triple{first_value, second_value, third_value});
                }
            }
        }
    }

    for (auto& tr : triplets) {
        std::sort(tr.begin(), tr.end());
    }
    std::sort(triplets.begin(), triplets.end());

    auto last_unique = std::unique(triplets.begin(), triplets.end());

    triplets.erase(last_unique, triplets.end());

    return triplets;
}

// @NOTE: Obviously could be optimized, but is only used for testing.
template <typename T>
static void assert_orderless_match(
    std::vector<Triple<T>> const& lhs,
    std::vector<Triple<T>> const& rhs) {
    size_t count = lhs.size();
    assert_eq(count, rhs.size());

    bool does_match = true;
    for (size_t lhs_idx = 0; lhs_idx < count; ++lhs_idx) {
        bool found = false;

        auto const& lhs_triple = lhs[lhs_idx];

        for (size_t rhs_idx = 0; (rhs_idx < count) && !found; ++rhs_idx) {
            auto const& rhs_triple = rhs[rhs_idx];

            bool triple_matches = true;
            triple_matches &= (lhs_triple[0] == rhs_triple[0]);
            triple_matches &= (lhs_triple[1] == rhs_triple[1]);
            triple_matches &= (lhs_triple[2] == rhs_triple[2]);

            found = triple_matches;
        }

        does_match &= found;

        if (!does_match) {
            break;
        }
    }

    assert(does_match);
}

int main() {
    assert_orderless_match(three_sum({-1, 0, 1, 2, -1, -4}, 0), {{-1, -1, 2}, {-1, 0, 1}});
    assert_orderless_match(three_sum({0, 1, 1}, 0), {});
    assert_orderless_match(three_sum({0, 0, 0}, 0), {{0, 0, 0}});

    report_success();
    return 0;
}
