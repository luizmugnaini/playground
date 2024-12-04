// ------------------------------------------------------------------------------------------------
// Title: Evaluate reverse Polish notation.
// Category: Medium.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// You are given an array of strings tokens that represents a valid arithmetic expression in Reverse Polish Notation.
//
// Return the integer that represents the evaluation of the expression.
//
// - The operands may be integers or the results of other operations.
// - The operators include '+', '-', '*', and '/'.
// - Assume that division between integers always truncates toward zero.
// ------------------------------------------------------------------------------------------------

#include <string>
#include <vector>
#include <common.hpp>

int32_t eval_rpn(std::vector<std::string> const& tokens) {
    std::vector<int32_t> stack = {};
    stack.reserve(tokens.size());

    for (auto const& tk : tokens) {
        size_t stack_count = stack.size();
        if (tk == "+") {
            assert(stack_count >= 2);

            int32_t second_operand = stack[stack_count - 1];
            int32_t first_operand  = stack[stack_count - 2];

            stack[stack_count - 2] = first_operand + second_operand;
            stack.pop_back();
        } else if (tk == "-") {
            assert(stack_count >= 2);

            int32_t second_operand = stack[stack_count - 1];
            int32_t first_operand  = stack[stack_count - 2];

            stack[stack_count - 2] = first_operand - second_operand;
            stack.pop_back();
        } else if (tk == "*") {
            assert(stack_count >= 2);

            int32_t second_operand = stack[stack_count - 1];
            int32_t first_operand  = stack[stack_count - 2];

            stack[stack_count - 2] = first_operand * second_operand;
            stack.pop_back();
        } else if (tk == "/") {
            assert(stack_count >= 2);

            int32_t second_operand = stack[stack_count - 1];
            int32_t first_operand  = stack[stack_count - 2];

            stack[stack_count - 2] = first_operand / second_operand;
            stack.pop_back();
        } else {
            try {
                stack.push_back(std::stoi(tk));
            } catch (std::invalid_argument const& error) {
                std::cout << "Failed to parse reverse Polish notation with error: " << error.what() << "\n";
            } catch (std::out_of_range const& error) {
                std::cout << "Unable to fit " << tk << " into an int32_t: " << error.what() << "\n";
            }
        }
    }

    return stack.back();
}

int main() {
    assert_eq(eval_rpn({"1", "2", "+", "3", "*", "4", "-"}), 5);
    assert_eq(eval_rpn({"2", "1", "+", "3", "*"}), 9);
    assert_eq(eval_rpn({"4", "13", "5", "/", "+"}), 6);

    report_success();
    return 0;
}
