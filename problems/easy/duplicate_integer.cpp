// Title: Duplicate integer.
// Category: Easy.
//
// Description:
//
// Given an integer array, return true if any value appears more than once in the array, otherwise
// return false.
//
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#include <cassert>
#include <set>
#include <type_traits>
#include <vector>

template <typename T>
    requires std::is_trivially_copyable_v<T>
bool has_duplicate(std::vector<T>& numbers) noexcept {
    std::set<T> unique_values;

    for (T const& val : numbers) {
        auto [iter, success] = unique_values.insert(val);
        if (!success) {
            break;
        }
    }

    return (unique_values.size() != numbers.size());
}

int main(int argc, char** argv) {
    std::vector<int> x0 = {1, 2, 12739, 117823, 289739, 120, 19287, 37287, 12739, 33333};
    assert(has_duplicate(x0));

    std::vector<int> x1 = {1, 2, 12739, 117823, 289739, 120, 19287, 37287, 33333};
    assert(!has_duplicate(x1));

    std::printf("%s: Success!", argv[0]);
    return 0;
}
