// Title: Is anagram.
// Category: Easy.
//
//
// Description:
//
// Given two strings s and t, return true if the two strings are anagrams of each other, otherwise
// return false.
//
// An anagram is a string that contains the exact same characters as another string, but the order
// of the characters can be different.
//
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#include <array>
#include <cassert>
#include <limits>
#include <string_view>

bool is_anagram(std::string_view lhs, std::string_view rhs) noexcept {
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

int main(int argc, char** argv) {
    assert(is_anagram("racecar", "carrace"));
    assert(is_anagram("uliz", "luiz"));

    assert(is_anagram("c+++++", "++c+++"));
    assert(!is_anagram("c+++++", "++cc+++"));

    assert(is_anagram("*&##@$", "$##&*@"));
    assert(!is_anagram("*&##@$", "$##&$*@"));

    assert(!is_anagram("jar", "jam"));

    std::printf("%s: Success!", argv[0]);
    return 0;
}
