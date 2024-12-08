#include "common.h"

static void quick_sort_u32_range(u32* start, u32* end) {
    if (start >= end) {
        return;
    }

    u32* left_scan  = start + 1;
    u32* right_scan = end;
    for (;;) {
        while (*left_scan <= *start) {
            if (left_scan == end) {
                break;
            }
            ++left_scan;
        }

        while (*start <= *right_scan) {
            if (right_scan == start) {
                break;
            }
            --right_scan;
        }

        if (right_scan <= left_scan) {
            break;
        }

        swap_u32_values(*left_scan, *right_scan);
    }
    swap_u32_values(*start, *right_scan);

    quick_sort_u32_range(start, right_scan - 1);
    quick_sort_u32_range(right_scan + 1, end);
}

yo_internal isize impl_binary_search_u32_values(u32 const* values, u32 match_value, isize low, isize high) {
    if (yo_unlikely(high < low)) {
        return -1;
    }

    isize mid       = (low + high) / 2;
    u32   mid_value = values[mid];

    if (mid_value == match_value) {
        return yo_cast(isize, mid);
    }

    isize new_low, new_high;
    if (mid_value > match_value) {
        new_low  = low;
        new_high = mid - 1;
    } else {
        new_low  = mid + 1u;
        new_high = high;
    }

    return impl_binary_search_u32_values(values, match_value, new_low, new_high);
}

isize binary_search_u32_values(u32 const* values, usize count, u32 match_value) {
    return impl_binary_search_u32_values(values, match_value, 0, yo_cast(isize, count) - 1);
}
