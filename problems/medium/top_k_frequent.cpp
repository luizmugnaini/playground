// ------------------------------------------------------------------------------------------------
// Title: Top k elements.
// Category: Medium.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// Given an integer array nums and an integer k, return the k most frequent elements within the
// array.
//
// You may assume that the answer is unique.
//
// The output may be returned in any order.
// ------------------------------------------------------------------------------------------------

#include <algorithm>
#include <common.hpp>
#include <set>
#include <unordered_map>
#include <vector>

template <typename T>
static std::vector<T> top_frequent(std::vector<T> const& values, size_t frequent_values_count) {
    auto value_count = values.size();

    // Record the element frequencies.
    std::unordered_map<T, size_t> frequency_record;
    frequency_record.reserve(value_count);
    for (auto& val : values) {
        ++frequency_record[val];
    }

    // Sort the map by the frequencies.
    std::vector<std::pair<T, size_t>> sorted_by_frequency{frequency_record.begin(), frequency_record.end()};
    std::sort(sorted_by_frequency.begin(), sorted_by_frequency.end(), [](auto const& lhs, auto const& rhs) {
        return lhs.second > rhs.second;
    });

    auto result_values_count = std::min(sorted_by_frequency.size(), frequent_values_count);

    // Get the topmost frequent values.
    std::vector<T> result;
    result.reserve(result_values_count);
    for (size_t idx = 0; idx < result_values_count; ++idx) {
        result.push_back(sorted_by_frequency[idx].first);
    }

    return result;
}

int main() {
    // Test 1.
    {
        std::vector<int32_t> values = {1, 2, 2, 3, 3, 3};

        auto result1 = top_frequent(values, 1);
        assert(debug::orderless_match({3}, result1));

        auto result2 = top_frequent(values, 2);
        assert(debug::orderless_match({2, 3}, result2));

        auto result3 = top_frequent(values, 3);
        assert(debug::orderless_match({3, 1, 2}, result3));

        auto result4 = top_frequent(values, 4);
        assert(debug::orderless_match(result3, result4));
    }

    // Test 2.
    {
        std::vector<std::string> values = {
            "lord of the rings",
            "hobbit",
            "half life",
            "pokemon",
            "pokemon",
            "hobbit",
            "pokemon",
            "half life 2",
            "lord of the rings",
            "hobbit",
            "arthur schopenhauer",
            "pokemon",
        };
        std::set<std::string>    set_values{values.begin(), values.end()};
        std::vector<std::string> unique_values{set_values.begin(), set_values.end()};

        auto result1 = top_frequent(values, 1);
        assert(debug::orderless_match({"pokemon"}, result1));

        auto result2 = top_frequent(values, 2);
        assert(debug::orderless_match({"pokemon", "hobbit"}, result2));

        auto result3 = top_frequent(values, 3);
        assert(debug::orderless_match({"lord of the rings", "hobbit", "pokemon"}, result3));

        auto result_too_much = top_frequent(values, values.size() + 11);
        assert(debug::orderless_match(unique_values, result_too_much));
    }

    report_success();

    return 0;
}
