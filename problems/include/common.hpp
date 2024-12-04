// Description: Utilities common to all solutions.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

#define max_value(lhs, rhs) (((lhs) >= (rhs)) ? (lhs) : (rhs))
#define min_value(lhs, rhs) (((lhs) <= (rhs)) ? (lhs) : (rhs))

#define assert_eq(lhs, rhs)                                                                \
    do {                                                                                   \
        auto lhs_value_ = (lhs);                                                           \
        auto rhs_value_ = static_cast<decltype(lhs_value_)>(rhs);                          \
        if (lhs_value_ != rhs_value_) {                                                    \
            std::cout << "[ERROR][" << __builtin_FILE() << ":" << __builtin_LINE() << "] " \
                      << lhs_value_ << " != " << rhs_value_                                \
                      << " for assertion " << #lhs << " == " << #rhs << "\n";              \
            std::abort();                                                                  \
        }                                                                                  \
    } while (0)

#define report_success() std::cout << "[INFO][" << __builtin_FILE() << "] Tests passed!\n"

namespace debug {
    template <typename T>
    void print_vector(std::vector<T> const& vec) {
        std::cout << "{";
        for (T const& e : vec) {
            std::cout << e << ", ";
        }
        std::cout << "}\n";
    }
}  // namespace debug
