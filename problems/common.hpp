// Description: Utilities common to all solutions.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>

#define assert_eq(lhs, rhs)                                                                \
    do {                                                                                   \
        auto lhs_value_ = (lhs);                                                           \
        auto rhs_value_ = (rhs);                                                           \
        if (lhs_value_ != rhs_value_) {                                                    \
            std::cout << "[ERROR][" << __builtin_FILE() << ":" << __builtin_LINE() << "] " \
                      << lhs_value_ << " != " << rhs_value_                                \
                      << " for assertion " << #lhs << " == " << #rhs << "\n";              \
            std::abort();                                                                  \
        }                                                                                  \
    } while (0)

#define report_success() std::cout << "[INFO][" << __builtin_FILE() << "] Tests passed!\n"
