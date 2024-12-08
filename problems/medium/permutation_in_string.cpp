// ------------------------------------------------------------------------------------------------
// Title: Permutation in string.
// Category: Medium.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// You are given two strings s1 and s2.
//
// Return true if s2 contains a permutation of s1, or false otherwise. That means if a permutation
// of s1 exists as a substring of s2, then return true.
//
// Both strings only contain lowercase letters.
// ------------------------------------------------------------------------------------------------

#include <common.hpp>
#include <cstring>
#include <string>
#include <vector>

constexpr size_t FREQUENCIES_COUNT = 'z' - 'a' + 1;

static inline bool have_equal_frequencies(uint32_t lhs[FREQUENCIES_COUNT], uint32_t rhs[FREQUENCIES_COUNT]) {
    bool matches = true;
    for (size_t idx = 0; idx < FREQUENCIES_COUNT; ++idx) {
        matches &= (lhs[idx] == rhs[idx]);
    }
    return matches;
};

static bool has_permutation_substring_of(std::string const& target_substr, std::string const& str) {
    size_t target_length = target_substr.size();
    size_t str_length    = str.size();
    if (str_length < target_length) {
        return false;
    }

    uint32_t target_frequencies[FREQUENCIES_COUNT] = {};

    for (char c : target_substr) {
        ++target_frequencies[c - 'a'];
    }

    uint32_t substr_frequencies[FREQUENCIES_COUNT] = {};

    // Compute the first substring character frequency.
    for (size_t idx = 0; idx < target_length; ++idx) {
        ++substr_frequencies[str[idx] - 'a'];
    }

    bool permutation_substr_found = have_equal_frequencies(substr_frequencies, target_frequencies);

    // Advance the substring character by character.
    for (size_t substr_last = target_length; !permutation_substr_found && (substr_last < str_length); ++substr_last) {
        // Remove the current first character of the substring.
        char first_char = str[substr_last - target_length];
        --substr_frequencies[first_char - 'a'];

        // Add the next character to the substring.
        char next_char = str[substr_last];
        ++substr_frequencies[next_char - 'a'];

        permutation_substr_found = have_equal_frequencies(substr_frequencies, target_frequencies);
    }

    return permutation_substr_found;
}

int main() {
    assert(has_permutation_substring_of("abc", "aldfafcbaionm"));
    assert(has_permutation_substring_of("abc", "aldfafcabionm"));
    assert(has_permutation_substring_of("abc", "aldfafabcionm"));
    assert(has_permutation_substring_of("adc", "dcda"));
    assert(!has_permutation_substring_of("ab", "a"));
    assert(!has_permutation_substring_of("abc", "car"));
    assert(!has_permutation_substring_of("abc", "bablac"));

    report_success();
    return 0;
}
