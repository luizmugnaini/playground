// ------------------------------------------------------------------------------------------------
// Title: Generate parentheses.
// Category: Medium.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// You are given an integer n. Return all well-formed parentheses strings that you can generate
// with n pairs of parentheses.
// ------------------------------------------------------------------------------------------------

#include <common.hpp>
#include <string>
#include <vector>

static void valid_parentheses_string_breadth_first_search(
    std::vector<std::string>& result,
    std::string&              str,
    uint64_t                  open_count,
    uint64_t                  closed_count,
    uint64_t                  pair_count) {
    if (open_count == pair_count && closed_count == pair_count) {
        // Copy the valid string to the result array.
        result.push_back(str);
        return;
    }

    if (open_count < pair_count) {
        str.push_back('(');

        valid_parentheses_string_breadth_first_search(
            result,
            str,
            open_count + 1,
            closed_count,
            pair_count);

        str.pop_back();
    }

    if (closed_count < open_count) {
        str.push_back(')');

        valid_parentheses_string_breadth_first_search(
            result,
            str,
            open_count,
            closed_count + 1,
            pair_count);

        str.pop_back();
    }
}

static std::vector<std::string> generate_parentheses(uint64_t pair_count) {
    if (pair_count == 0) {
        return {};
    }

    std::vector<std::string> result;

    std::string base;
    base.reserve(2 * pair_count);

    valid_parentheses_string_breadth_first_search(result, base, 0, 0, pair_count);

    return result;
}

int main() {
    assert(debug::orderless_match(generate_parentheses(1), {"()"}));
    assert(debug::orderless_match(generate_parentheses(2), {"()()", "(())"}));
    assert(debug::orderless_match(generate_parentheses(3), {"((()))", "(()())", "(())()", "()(())", "()()()"}));

    report_success();
    return 0;
}
