// ------------------------------------------------------------------------------------------------
// Title: Minimum Stack.
// Category: Medium.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// Design a stack class that supports the push, pop, top, and getMin operations.
//
// - MinStack() initializes the stack object.
// - void push(int val) pushes the element val onto the stack.
// - void pop() removes the element on the top of the stack.
// - int top() gets the top element of the stack.
// - int getMin() retrieves the minimum element in the stack.
//
// Each function should run in O(1) time.
// ------------------------------------------------------------------------------------------------

#include <utility>
#include <vector>
#include <common.hpp>

template <typename T>
struct MinStack {
    MinStack() = default;

    void push(T value) {
        if (stack.empty()) {
            stack.push_back(std::make_pair(value, value));
        } else {
            stack.push_back(std::make_pair(value, min_value(stack.back().second, value)));
        }
    }

    void pop() {
        if (!stack.empty()) {
            stack.pop_back();
        }
    }

    T top() {
        return stack.back().first;
    }

    T get_min() {
        return stack.back().second;
    }

private:
    std::vector<std::pair<T, T>> stack;
};

int main() {
    MinStack<int32_t> s{};
    s.push(9);
    assert_eq(s.top(), 9);
    assert_eq(s.get_min(), 9);
    s.push(3);
    assert_eq(s.top(), 3);
    assert_eq(s.get_min(), 3);
    s.push(-2);
    assert_eq(s.top(), -2);
    assert_eq(s.get_min(), -2);
    s.push(1);
    assert_eq(s.top(), 1);
    assert_eq(s.get_min(), -2);
    s.push(7);
    assert_eq(s.top(), 7);
    assert_eq(s.get_min(), -2);

    report_success();
    return 0;
}
