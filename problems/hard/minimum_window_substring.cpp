// ------------------------------------------------------------------------------------------------
// Title: Minimum window substring.
// Category: Hard.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// Given two strings s and t, return the shortest substring of s such that every character in t,
// including duplicates, is present in the substring. If such a substring does not exist, return
// an empty string "".
//
// You may assume that the correct output is always unique.
// ------------------------------------------------------------------------------------------------

#include <common.hpp>
#include <string>
#include <vector>

static constexpr size_t ALPHABET_COUNT    = 'z' - 'a' + 1u;
static constexpr size_t FREQUENCIES_COUNT = 2u * ALPHABET_COUNT;

static inline size_t char_to_frequencies_index(char c) {
    return (('A' <= c) && (c <= 'Z')) ? (static_cast<size_t>(c) - 'A' + ALPHABET_COUNT) : (static_cast<size_t>(c) - 'a');
}

static std::string minimum_window_substring(std::string const& str, std::string const& target) {
    size_t target_length = target.size();
    size_t str_length    = str.size();

    if ((str_length == 0) || (target_length == 0) || (str_length < target_length)) {
        return "";
    }

    // Counts of a given character that still needs to be included in the substring.
    // If negative, then the current substring has an exceeding frequency of that character.
    int32_t remaining_frequencies[FREQUENCIES_COUNT] = {};
    for (size_t idx = 0; idx < target_length; ++idx) {
        ++remaining_frequencies[char_to_frequencies_index(target[idx])];
    }

    size_t target_remaining_chars_count = target_length;

    // We choose the end to be str_length + 1 in order to forcefully swap the minimal substring
    // in the first iteration.
    size_t min_substr_start = 0;
    size_t min_substr_end   = str_length + 1;

    for (size_t start = 0, end = 1; end <= str_length; ++end) {
        // Acquire the character prior to the end into the substring.
        {
            size_t end_char_as_idx = char_to_frequencies_index(str[end - 1]);

            // If the end char is in the target, account for it in the remaining characters.
            if (remaining_frequencies[end_char_as_idx] > 0) {
                --target_remaining_chars_count;
            }

            // Add the character to the frequency, regardless if it's in the target string.
            --remaining_frequencies[end_char_as_idx];
        }

        // If our substring encompasses every character we need, try to make it smaller by moving
        // the start character forwards.
        if (target_remaining_chars_count == 0) {
            for (;;) {
                size_t start_char_as_idx = char_to_frequencies_index(str[start]);

                if (remaining_frequencies[start_char_as_idx] == 0) {
                    // Cannot continue moving the start character, as it is necessary to match the
                    // frequency of the target.
                    break;
                }

                // Remove the current start character and move it forwards.
                ++remaining_frequencies[start_char_as_idx];
                ++start;
            }

            // Check if the current substring is smaller than the last one.
            if (end - start < min_substr_end - min_substr_start) {
                min_substr_start = start;
                min_substr_end   = end;
            }

            // Remove the current start character and move it forwards.
            ++remaining_frequencies[char_to_frequencies_index(str[start])];
            ++target_remaining_chars_count;
            ++start;
        }
    }

    return min_substr_end > str_length ? "" : str.substr(min_substr_start, min_substr_end - min_substr_start);
}

int main() {
    assert_eq(minimum_window_substring("OUZODYXAZV", "XYZ"), "YXAZ");
    assert_eq(minimum_window_substring("ADOBECODEBANC", "ABC"), "BANC");
    assert_eq(minimum_window_substring("xyz", "xyz"), "xyz");
    assert_eq(minimum_window_substring("x", "xy"), "");
    assert_eq(minimum_window_substring("a", "aa"), "");

    report_success();
    return 0;
}
