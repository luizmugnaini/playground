#ifndef AOC24_COMMON_H
#define AOC24_COMMON_H

#include <yoneda_core.h>

#define make_input_path(day, part) ("aoc24/input/day" day "_part" part ".txt")

#define swap_u32_values(lhs, rhs) \
    do {                          \
        u32 temp_ = (lhs);        \
        (lhs)     = (rhs);        \
        (rhs)     = temp_;        \
    } while (0)

typedef struct Result {
    u64 result;
    f64 elapsed_time;
} Result;

static void quick_sort_u32_range(u32* start, u32* end);

isize binary_search_u32_values(u32 const* values, usize count, u32 match_value);

#endif  // AOC24_COMMON_H
