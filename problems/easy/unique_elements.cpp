// ------------------------------------------------------------------------------------------------
// Title: Unique elements.
// Category: Easy.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// Problem 1: Given an array, return an array containing the non-repeating elements of the original
//            array.
// Problem 2: Solve problem 1 inplace and return the size of the subarray that contains all unique
//            elements. The subarray should start at index 0 of the array.
// ------------------------------------------------------------------------------------------------

#include <algorithm>
#include <cstdlib>
#include <type_traits>
#include <unordered_set>
#include <vector>
#include "../common.hpp"

// ------------------------------------------------------------------------------------------------
// Problem 1 solutions.
// ------------------------------------------------------------------------------------------------

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
static std::vector<T> unique_elements_using_unordered_set(std::vector<T> const& values) {
    std::unordered_set<T> unique_values{values.begin(), values.end()};
    return {unique_values.begin(), unique_values.end()};
}

// ------------------------------------------------------------------------------------------------
// Problem 2 solutions.
// ------------------------------------------------------------------------------------------------

#define swap_values(lhs, rhs) \
    do {                      \
        auto temp_ = rhs;     \
        rhs        = lhs;     \
        lhs        = temp_;   \
    } while (0)

template <typename T>
    requires std::is_trivially_copyable_v<T>
static size_t unique_elements_inplace(std::vector<T>& values) {
    size_t count = values.size();
    if (count == 0) {
        return 0;
    }

    size_t unique_values_count = 1;

    auto find_until = [&](T value) -> bool {
        for (size_t idx = 0; idx < unique_values_count; ++idx) {
            if (value == values[idx]) {
                return true;
            }
        }
        return false;
    };

    size_t end = count;
    for (size_t idx = 1; idx < end;) {
        if (find_until(values[idx])) {
            swap_values(values.begin() + idx, values.begin() + end - 1);
            --end;
            continue;
        }
        ++unique_values_count;
        ++idx;
    }

    return unique_values_count;
}

template <typename T>
    requires std::is_trivially_copyable_v<T>
static size_t unique_elements_inplace_sort_using_sort_and_unique(std::vector<T>& values) {
    size_t count = values.size();
    if (count == 0) {
        return 0;
    }

    auto values_begin = values.begin();
    auto values_end   = values.end();

    std::sort(values_begin, values_end);
    auto unique_values_end = std::unique(values_begin, values_end);

    return std::distance(values.begin(), unique_values_end);
}

// ------------------------------------------------------------------------------------------------
// Auxiliar procedures for testing.
// ------------------------------------------------------------------------------------------------

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
static void assert_orderless_equal_vectors_up_to(std::vector<T> lhs, std::vector<T> rhs, size_t end) {
    for (size_t lhs_idx = 0; lhs_idx < end; ++lhs_idx) {
        bool found = false;
        for (size_t rhs_idx = 0; rhs_idx < end; ++rhs_idx) {
            if (lhs[lhs_idx] == rhs[rhs_idx]) {
                found = true;
                break;
            }
        }
        assert(found);
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

        // Problem 1.
        assert_orderless_equal_vectors(copy_unique_elements(values), expected);
        assert_equal_vectors(unique_elements_naive(values), expected);
        assert_orderless_equal_vectors(unique_elements_using_unordered_set(values), expected);

        // Problem 2.
        auto unique_values_count = unique_elements_inplace_sort_using_sort_and_unique(values);
        assert_eq(unique_values_count, expected.size());
        assert_orderless_equal_vectors_up_to(values, expected, unique_values_count);
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

            // Problem 1.
            assert_equal_vectors(unique_elements_naive(values), expected);
            assert_orderless_equal_vectors(unique_elements_using_unordered_set(values), expected);

            // Problem 2.
            {
                std::vector<int> copy{values.begin(), values.end()};

                auto unique_values_count = unique_elements_inplace(values);
                assert_eq(unique_values_count, expected.size());
                assert_orderless_equal_vectors_up_to(values, expected, unique_values_count);

                unique_values_count = unique_elements_inplace_sort_using_sort_and_unique(copy);
                assert_eq(unique_values_count, expected.size());
                assert_orderless_equal_vectors_up_to(copy, expected, unique_values_count);
            }
        }
    }

    report_success();
    return 0;
}
