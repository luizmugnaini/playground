// ------------------------------------------------------------------------------------------------
// Title: Products of array except self.
// Category: Medium.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// Given an integer array nums, return an array output where output[i] is the product of all the
// elements of nums except nums[i].
//
// Follow-up: Could you solve it in O(n)O(n) time without using the division operation?
// ------------------------------------------------------------------------------------------------

#include <vector>
#include "../common.hpp"

template <typename T>
static std::vector<T> product_except_self(std::vector<T> const& values) {
    size_t value_count = values.size();

    std::vector<T> results;
    results.reserve(value_count);

    for (size_t idx = 0; idx < value_count; ++idx) {
        T prod = 1;

        for (size_t left = 0; left < idx; ++left) {
            prod *= values[left];
        }

        for (size_t right = idx + 1; right < value_count; ++right) {
            prod *= values[right];
        }

        results.push_back(prod);
    }

    return results;
}

template <typename T>
static bool ordered_match(std::vector<T> const& lhs, std::vector<T> const& rhs) {
    size_t count = lhs.size();
    if (rhs.size() != count) {
        return false;
    }

    bool matches = true;
    for (size_t idx = 0; idx < count; ++idx) {
        matches &= (lhs[idx] == rhs[idx]);
    }

    return matches;
}

int main() {
    // Test 1.
    {
        std::vector<uint32_t> values   = {1, 2, 4, 6};
        std::vector<uint32_t> expected = {48, 24, 12, 8};
        auto                  results  = product_except_self(values);
        assert(ordered_match(expected, results));
    }

    // Test 2.
    {
        std::vector<int32_t> values   = {-1, 0, 1, 2, 3};
        std::vector<int32_t> expected = {0, -6, 0, 0, 0};
        auto                 results  = product_except_self(values);
        assert(ordered_match(expected, results));
    }

    report_success();
    return 0;
}
