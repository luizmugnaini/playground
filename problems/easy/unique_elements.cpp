// ------------------------------------------------------------------------------------------------
// Title: Unique elements.
// Category: Easy.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// Given an array, return an array containing the non-repeating elements of the original array.
// ------------------------------------------------------------------------------------------------

#include <algorithm>
#include <cstdlib>
#include <set>
#include <type_traits>
#include <vector>
#include "../common.hpp"

template <typename T>
    requires std::is_trivially_copyable_v<T>
static std::vector<T> unique_elements_naive(std::vector<T> const& values) {
    size_t count = values.size();
    if (count == 0) {
        return {};
    }

    std::vector<T> unique_values;
    unique_values.reserve(count);

    auto contains = [&](T v) -> bool {
        for (auto uv : unique_values) {
            if (uv == v) {
                return true;
            }
        }
        return false;
    };

    unique_values.push_back(values[0]);
    for (size_t idx = 1; idx < count; ++idx) {
        T value = values[idx];
        if (!contains(value)) {
            unique_values.push_back(value);
        }
    }

    return unique_values;
}

template <typename T>
    requires std::is_trivially_copyable_v<T>
static std::vector<T> unique_elements_set(std::vector<T> const& values) {
    std::set<T> unique_values{values.begin(), values.end()};
    return {unique_values.begin(), unique_values.end()};
}

template <typename T>
    requires std::is_trivially_copyable_v<T>
static std::vector<T> copy_unique_elements(std::vector<T> values) {
    std::sort(values.begin(), values.end());
    auto unique_values_end = std::unique(values.begin(), values.end());
    values.erase(unique_values_end, values.end());
    return values;
}

template <typename T>
static void assert_equal_vectors(std::vector<T> const& lhs, std::vector<T> const& rhs) {
    size_t count = lhs.size();
    assert_eq(count, rhs.size());

    for (size_t idx = 0; idx < count; ++idx) {
        assert_eq(lhs[idx], rhs[idx]);
    }
}

template <typename T>
static void assert_orderless_equal_vectors(std::vector<T> lhs, std::vector<T> rhs) {
    for (T const& l : lhs) {
        bool found = false;
        for (T const& r : rhs) {
            if (l == r) {
                found = true;
                break;
            }
        }
        assert(found);
    }
}

int main() {
    // Test 1.
    {
        std::vector values   = {1, 1, 2, 3, 454, 3, 5, 454, 88, 99, 88, 88};
        std::vector expected = {1, 2, 3, 454, 5, 88, 99};
        assert_orderless_equal_vectors(copy_unique_elements(values), expected);
        assert_equal_vectors(unique_elements_naive(values), expected);
        assert_orderless_equal_vectors(unique_elements_set(values), expected);
    }

    // Test 2.
    {
        std::vector<int> values;
        values.reserve(512);

        for (size_t iter = 0; iter < 100; ++iter) {
            for (auto& v : values) {
                v = std::rand();
            }

            auto expected = copy_unique_elements(values);

            assert_equal_vectors(unique_elements_naive(values), expected);
            assert_orderless_equal_vectors(unique_elements_set(values), expected);
        }
    }

    report_success();
    return 0;
}
