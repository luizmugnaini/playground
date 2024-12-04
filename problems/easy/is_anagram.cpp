// ------------------------------------------------------------------------------------------------
// Title: Is anagram.
// Category: Easy.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// Given two strings s and t, return true if the two strings are anagrams of each other, otherwise
// return false.
//
// An anagram is a string that contains the exact same characters as another string, but the order
// of the characters can be different.
// ------------------------------------------------------------------------------------------------

#include <array>
#include <limits>
#include <string_view>
#include <common.hpp>

static bool is_anagram(std::string_view lhs, std::string_view rhs) noexcept {
    constexpr size_t MAX_UNIQUE_CHARS = std::numeric_limits<char>::max();

    std::array<size_t, MAX_UNIQUE_CHARS> lhs_count = {};
    std::array<size_t, MAX_UNIQUE_CHARS> rhs_count = {};

    size_t char_count = lhs.size();
    if (char_count != rhs.size()) {
        return false;
    }

    for (size_t idx = 0; idx < char_count; ++idx) {
        ++lhs_count[static_cast<size_t>(lhs[idx])];
        ++rhs_count[static_cast<size_t>(rhs[idx])];
    }

    bool are_anagrams = true;
    for (char c : lhs) {
        if (lhs_count[static_cast<size_t>(c)] != rhs_count[static_cast<size_t>(c)]) {
            are_anagrams = false;
            break;
        }
    }

    return are_anagrams;
}

int main() {
    assert(is_anagram("racecar", "carrace"));
    assert(is_anagram("uliz", "luiz"));

    assert(is_anagram("c+++++", "++c+++"));
    assert(!is_anagram("c+++++", "++cc+++"));

    assert(is_anagram("*&##@$", "$##&*@"));
    assert(!is_anagram("*&##@$", "$##&$*@"));

    assert(!is_anagram("jar", "jam"));

    report_success();
    return 0;
}
