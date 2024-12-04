// ------------------------------------------------------------------------------------------------
// Title: Longest repeating character replacement.
// Category: Medium.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// You are given a string consisting of only uppercase english characters and an integer k. You
// can choose up to k characters of the string and replace them with any other uppercase English
// character.
//
// After performing at most k replacements, return the length of the longest substring which
// contains only one distinct character.
// ------------------------------------------------------------------------------------------------

#include <iterator>
#include <string>
#include <vector>
#include <common.hpp>

constexpr size_t CHARACTER_COUNT = 'Z' - 'A' + 1;

size_t character_replacement(std::string const& str, size_t max_replacements) {
    auto const str_end = str.end();

    size_t char_count_in_substring[CHARACTER_COUNT] = {};
    size_t max_count_in_substring                   = 0;

    auto   left                 = str.begin();
    size_t max_substring_length = 0;
    for (auto right = left; right != str_end; ++right) {
        char right_count_idx = *right - 'A';

        // Replace the right character by the corresponding one between A-Z.
        size_t char_last_count = char_count_in_substring[right_count_idx];
        ++char_count_in_substring[right_count_idx];
        max_count_in_substring = max_value(max_count_in_substring, char_last_count + 1);

        // We always replace with the character with maximum count in the string, as far as it doesn't
        // surpass the maximum number of replacements.
        size_t substring_length  = std::distance(left, right) + 1;
        size_t replacement_count = substring_length - max_count_in_substring;

        if (replacement_count <= max_replacements) {
            // Still a valid substring, account for it and advance the right character.
            max_substring_length = max_value(max_substring_length, substring_length);
        } else {
            // Not a valid substring. We can't replace with the most frequent character. Move the left
            // character forwards and decrease its frequency.
            --char_count_in_substring[*left - 'A'];
            ++left;
        }
    }

    return max_substring_length;
}

int main() {
    assert_eq(character_replacement("XYYX", 2u), 4u);
    assert_eq(character_replacement("ABAB", 2u), 4u);
    assert_eq(character_replacement("ABAB", 0u), 1u);
    assert_eq(character_replacement("AAAB", 0u), 3u);
    assert_eq(character_replacement("AABABBA", 1u), 4u);

    report_success();
    return 0;
}
