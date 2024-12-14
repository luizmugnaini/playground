// Description: Utilities common to all solutions.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

#define max_value(lhs, rhs) (((lhs) >= (rhs)) ? (lhs) : (rhs))
#define min_value(lhs, rhs) (((lhs) <= (rhs)) ? (lhs) : (rhs))

#define assert_eq(lhs, rhs)                                                                            \
    do {                                                                                               \
        auto                 lhs_value_ = (lhs);                                                       \
        decltype(lhs_value_) rhs_value_ = (rhs);                                                       \
        if (lhs_value_ != rhs_value_) {                                                                \
            std::cout << "[ERROR][" << __builtin_FILE() << ":" << __builtin_LINE() << "] "             \
                      << lhs_value_ << " != " << rhs_value_                                            \
                      << " for assertion (" << #lhs << " = " << lhs_value_ << ") == " << #rhs << "\n"; \
            std::abort();                                                                              \
        }                                                                                              \
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

    template <typename T>
    static bool orderless_match(std::vector<T> lhs, std::vector<T> rhs) {
        bool matches = true;
        for (T const& l : lhs) {
            bool found = false;
            for (T const& r : rhs) {
                if (l == r) {
                    found = true;
                    break;
                }
            }

            matches &= found;
            if (!matches) {
                break;
            }
        }
        return matches;
    }

    template <typename T>
    void vec_assert_eq(std::vector<T> const& lhs, std::vector<T> const& rhs) {
        size_t count = lhs.size();
        assert(rhs.size() == count);

        for (size_t idx = 0; idx < count; ++idx) {
            T lhs_value = lhs[idx];
            T rhs_value = rhs[idx];
            if (lhs_value != rhs_value) {
                std::cout << "[ERROR] Vectors don't match at " << idx
                          << " (lhs = " << lhs_value << ", rhs = " << rhs_value << ")\n";
                std::abort();
            }
        }
    }
}  // namespace debug
