// ------------------------------------------------------------------------------------------------
// Title: Search in a 2D matrix.
// Category: Medium.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// You are given an m x n 2-D integer array matrix and an integer target.
//
// - Each row in matrix is sorted in non-decreasing order.
// - The first integer of every row is greater than the last integer of the previous row.
//
// Return true if target exists within matrix or false otherwise.
//
// Can you write a solution that runs in O(log(m * n)) time?
// ------------------------------------------------------------------------------------------------

#include <common.hpp>
#include <type_traits>
#include <vector>

enum struct Compare {
    LESS,
    EQUAL,
    GREATER,
};

template <typename T>
    requires std::is_integral_v<T>
static Compare search_matrix_row_range(
    std::vector<T> const& row,
    T                     target_value,
    int64_t               start,
    int64_t               end,
    Compare               cmp = Compare::EQUAL) {
    if (end < start) {
        return cmp;
    }

    auto mid_idx = (end + start) / 2;
    auto mid     = row[mid_idx];

    if (mid == target_value) {
        cmp = Compare::EQUAL;
    } else if (mid <= target_value) {
        cmp = search_matrix_row_range(row, target_value, mid_idx + 1, end, Compare::LESS);
    } else if (mid >= target_value) {
        cmp = search_matrix_row_range(row, target_value, start, mid_idx - 1, Compare::GREATER);
    }

    return cmp;
}

template <typename T>
    requires std::is_integral_v<T>
static bool search_matrix_range(
    std::vector<std::vector<T>> const& matrix,
    T                                  target_value,
    int64_t                            start_row,
    int64_t                            end_row) {
    if (end_row < start_row) {
        return false;
    }

    auto        mid_row_idx = (end_row + start_row) / 2;
    auto const& mid_row     = matrix[mid_row_idx];

    bool found = false;
    auto cmp   = search_matrix_row_range(mid_row, target_value, 0, mid_row.size() - 1);
    switch (cmp) {
        case Compare::EQUAL:   found = true; break;
        case Compare::LESS:    found = search_matrix_range(matrix, target_value, mid_row_idx + 1, end_row); break;
        case Compare::GREATER: found = search_matrix_range(matrix, target_value, start_row, mid_row_idx - 1); break;
    }

    return found;
}

template <typename T>
    requires std::is_integral_v<T>
static bool search_matrix(std::vector<std::vector<T>> const& matrix, T target_value) {
    auto row_count = matrix.size();
    if (row_count == 0) {
        return false;
    }
    return search_matrix_range(matrix, target_value, 0, row_count - 1);
}

int main() {
    assert(search_matrix({{1, 2, 4, 8}, {10, 11, 12, 13}, {14, 20, 30, 40}}, 10));
    assert(search_matrix({{1, 3, 5, 7}, {10, 11, 16, 20}, {23, 30, 34, 60}}, 3));
    report_success();
    return 0;
}
