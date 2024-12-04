// ------------------------------------------------------------------------------------------------
// Title: Longest substring with non-repeating characters.
// Category: Medium.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// Given a string, find the length of the longest substring without duplicate characters.
//
// A substring is a contiguous sequence of characters within a string.
//
// @NOTE: I'll assume the string is composed of ASCII characters.
// ------------------------------------------------------------------------------------------------

#include <algorithm>
#include <array>
#include <string>
#include <common.hpp>

size_t length_of_longest_substring_with_unique_characters(std::string const& str) {
    auto length = static_cast<int64_t>(str.size());

    std::array<int64_t, 256> character_index_in_str;
    std::fill(character_index_in_str.begin(), character_index_in_str.end(), -1);

    int64_t left       = 0;
    size_t  max_length = 0;
    for (int64_t right = 0; right < length; ++right) {
        char    current_right = str[right];
        int64_t idx           = character_index_in_str[current_right];
        if (idx >= left) {
            left = idx + 1;
        }
        character_index_in_str[current_right] = right;

        max_length = max_value(max_length, static_cast<size_t>(right - left + 1));
    }

    return max_length;
}

int main() {
    assert_eq(length_of_longest_substring_with_unique_characters(""), 0);
    assert_eq(length_of_longest_substring_with_unique_characters("a"), 1);
    assert_eq(length_of_longest_substring_with_unique_characters("ab"), 2);
    assert_eq(length_of_longest_substring_with_unique_characters("aab"), 2);
    assert_eq(length_of_longest_substring_with_unique_characters("aaaaaaaa"), 1);
    assert_eq(length_of_longest_substring_with_unique_characters("zxyzxyz"), 3);
    assert_eq(length_of_longest_substring_with_unique_characters("pwwkew"), 3);

    report_success();
    return 0;
}
