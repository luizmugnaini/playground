// ------------------------------------------------------------------------------------------------
// Title: Valid parentheses.
// Category: Easy.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
// You are given a string s consisting of the following characters: '(', ')', '{', '}', '[' and ']'.
//
// The input string s is valid if and only if:
// - Every open bracket is closed by the same type of close bracket.
// - Open brackets are closed in the correct order.
// - Every close bracket has a corresponding open bracket of the same type.
//
// Return true if s is a valid string, and false otherwise.
// ------------------------------------------------------------------------------------------------

#include <string>
#include <vector>
#include "../common.hpp"

enum Parenthesis {
    PAREN = 0,
    CURLY,
    BRACKET,
    PARENTHESIS_COUNT,
};

bool parentheses_are_valid(std::string const& str) {
    size_t char_count = str.size();

    std::vector<Parenthesis> parentheses_stack;
    parentheses_stack.reserve(char_count);

    auto current  = str.begin();
    auto end      = str.end();
    bool is_valid = true;
    while (is_valid && current != end) {
        char c           = *current;
        bool stack_empty = parentheses_stack.empty();

        switch (c) {
            case '(':
                parentheses_stack.push_back(PAREN);
                break;
            case ')':
                if (!stack_empty && parentheses_stack.back() == PAREN) {
                    parentheses_stack.pop_back();
                } else {
                    is_valid = false;
                }
                break;
            case '{':
                parentheses_stack.push_back(CURLY);
                break;
            case '}':
                if (!stack_empty && parentheses_stack.back() == CURLY) {
                    parentheses_stack.pop_back();
                } else {
                    is_valid = false;
                }
                break;
            case '[':
                parentheses_stack.push_back(BRACKET);
                break;
            case ']':
                if (!stack_empty && parentheses_stack.back() == BRACKET) {
                    parentheses_stack.pop_back();
                } else {
                    is_valid = false;
                }
                break;
        }
        ++current;
    }

    return is_valid && parentheses_stack.empty();
}

int main() {
    assert(parentheses_are_valid("[]"));
    assert(parentheses_are_valid("{}([[(){}]])"));
    assert(parentheses_are_valid(""));
    assert(!parentheses_are_valid("["));
    assert(!parentheses_are_valid("[(])"));

    report_success();
    return 0;
}
