// ------------------------------------------------------------------------------------------------
// Title: Anagram groups.
// Category: Medium.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// Given an array of strings group all anagrams together into sublists. You may return the output in
// any order.
//
// An anagram is a string that contains the exact same characters as another string, but the order
// of the characters can be different.
// ------------------------------------------------------------------------------------------------

#include <cmath>
#include <common.hpp>
#include <limits>

static inline bool is_digit(char c) {
    return (('0' <= c) && (c <= '9'));
}

static int32_t string_to_integer(std::string const& str) {
    int32_t result = 0;

    auto str_end = str.end();

    // Skip all of the starting white-space.
    auto it = str.begin();
    while ((it < str_end) && ((*it) == ' ')) {
        ++it;
    }

    // Get the sign of the value.
    bool    is_valid = true;
    int32_t sign     = 1;
    if (it < str_end) {
        char c = *it;
        if (c == '-') {
            sign = -1;
            ++it;
        } else if (c == '+') {
            ++it;
        } else if (!is_digit(c)) {
            is_valid = false;
        }
    }

    // Acquire the digits into a number.
    if (sign == 1) {
        while (is_valid && (it < str_end)) {
            char c = *it;
            if (is_digit(c)) {
                int32_t digit = c - '0';
                if (10 * static_cast<int64_t>(result) > INT32_MAX - digit) {
                    result = INT32_MAX;
                    break;
                } else {
                    result = 10 * result + digit;
                    ++it;
                }
            } else {
                break;
            }
        }
    } else {
        while (is_valid && (it < str_end)) {
            char c = *it;
            if (is_digit(c)) {
                int32_t digit = c - '0';
                if (10 * static_cast<int64_t>(result) < INT32_MIN + digit) {
                    result = INT32_MIN;
                    break;
                } else {
                    result = 10 * result - digit;
                    ++it;
                }
            } else {
                break;
            }
        }
    }

    return result;
}

int main() {
    assert_eq(string_to_integer("1234"), 1234);
    assert_eq(string_to_integer("-1234"), -1234);
    assert_eq(string_to_integer("+1234"), 1234);
    assert_eq(string_to_integer("        -1234"), -1234);
    assert_eq(string_to_integer("  +1234"), 1234);
    assert_eq(string_to_integer("  - 1234"), 0);
    assert_eq(string_to_integer(" +   1234"), 0);
    assert_eq(string_to_integer("0-1"), 0);
    assert_eq(string_to_integer("words and 987"), 0);
    assert_eq(string_to_integer("-0046"), -46);
    assert_eq(string_to_integer("1337c0d3"), 1337);
    assert_eq(string_to_integer("-91283472332"), -2147483648);
    assert_eq(string_to_integer("-2147483648"), -2147483648);

    report_success();
    return 0;
}
