// ------------------------------------------------------------------------------------------------
// Title: Valid palindrome.
// Category: Easy.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// Given a string s, return true if it is a palindrome, otherwise return false.
//
// A palindrome is a string that reads the same forward and backward. It is also case-insensitive
// and ignores all non-alphanumeric characters.
// ------------------------------------------------------------------------------------------------

#include <cctype>
#include <string>
#include "../common.hpp"

static bool is_valid_palindrome(std::string const& str) {
   auto left     = str.begin();
    auto right    = str.end() - 1;

    bool is_valid = true;
    while (left <= right) {
        bool left_is_alphanumeric  = std::isalnum(*left);
        bool right_is_alphanumeric = std::isalnum(*right);

        left += static_cast<ptrdiff_t>(!left_is_alphanumeric);
        right -= static_cast<ptrdiff_t>(!right_is_alphanumeric);

        if (left > right) {
            break;
        }
        
        if (left_is_alphanumeric && right_is_alphanumeric) {
            if (std::tolower(*left) != std::tolower(*right)) {
                is_valid = false;
                break;

            }

            ++left;
            --right;
        }
    }

    return is_valid;
}

int main() {
    assert(is_valid_palindrome("!*(&(!@#! #&!#*!(# &#!(&(#!&(_*><<("));  // Yeah... kind of stupid.
    assert(is_valid_palindrome("Was it a car or a cat I saw?"));
    assert(!is_valid_palindrome("tab a cat"));

    report_success();
    return 0;
}
