#include <yoneda_bit.h>
#include <yoneda_memory.h>
#include <yoneda_string.h>
#include <yoneda_time.h>
#include "common.h"

#include <stdio.h>

typedef struct Day2Report {
    u32 count;
    u32 values[8];
} Day2Report;

static u64 day2_parse_input(yo_String input, Day2Report* reports, usize max_report_count, u32 max_values_per_report) {
    u64 report_count = 0;

    char const* input_end    = yo_cast(char const*, yo_ptr_add(input.buf, input.length));
    char const* current_char = yo_cast(char const*, input.buf);
    while ((current_char < input_end) && (report_count <= max_report_count)) {
        char number_buf[4];
        u32  number_buf_length = 0;

        Day2Report report = yo_make_default(Day2Report);

        // Parse the report values.
        while (report.count < max_values_per_report) {
            // Copy the number to the string buffer.
            number_buf_length = (current_char[1] == ' ' || yo_char_is_crlf(current_char[1])) ? 1 : 2;
            yo_memory_move(yo_cast(u8*, number_buf), yo_cast(u8 const*, current_char), number_buf_length);

            // Convert the number string to a uint.
            yo_assert(yo_string_to_u32((yo_String){number_buf, number_buf_length}, &report.values[report.count]));
            ++report.count;

            // Go to the next number or end parsing the current line.
            current_char += number_buf_length + 1;
            if (*(current_char - 1) != ' ') {
                break;
            }
        }

        // Record the report to the array.
        yo_memory_move(yo_cast(u8*, &reports[report_count]), yo_cast(u8 const*, &report), yo_size_of(Day2Report));
        ++report_count;

        // Skip to the next line and/or check if we reached the end of the file.
        {
            bool is_eof = (current_char == input_end);
            while (!is_eof && yo_char_is_crlf(*current_char)) {
                ++current_char;
                is_eof &= (current_char == input_end);
            }

            if (yo_unlikely(is_eof)) {
                break;
            }
        }
    }

    return report_count;
}

static Result day2_part1(yo_String input) {
    f64 start_time = yo_current_time_in_seconds();

    Day2Report reports[1000];
    u64        report_count = day2_parse_input(input, reports, yo_count_of(reports), yo_count_of(reports[0].values));

    u64 safe_report_count = 0;
    for (usize report_idx = 0; report_idx < report_count; ++report_idx) {
        Day2Report* report = &reports[report_idx];
        u32         count  = report->count;

        // @NOTE: We know that report->count will always be > 1.
        i32  last_diff = yo_cast(i32, report->values[1]) - yo_cast(i32, report->values[0]);
        bool is_safe   = yo_value_in_range(yo_i32_abs_value(last_diff), 1u, 3u);

        for (u32 value_idx = 1u; (value_idx < count - 1u) && is_safe; ++value_idx) {
            i32 diff = yo_cast(i32, report->values[value_idx + 1u]) - yo_cast(i32, report->values[value_idx]);

            is_safe &= !yo_i32_opposite_sign(diff, last_diff);
            is_safe &= yo_value_in_range(yo_i32_abs_value(diff), 1u, 3u);

            last_diff = diff;
        }

        safe_report_count += yo_cast(u64, is_safe);
    }

    f64 end_time = yo_current_time_in_seconds();

    return (Result){
        .result       = safe_report_count,
        .elapsed_time = end_time - start_time,
    };
}

static yo_inline bool day2_part2_is_diff_in_safe_range(i32 diff) {
    return yo_value_in_range(yo_i32_abs_value(diff), 1u, 3u);
}

static yo_inline bool day2_part2_is_diff_safe(i32 last_diff, i32 current_diff) {
    return day2_part2_is_diff_in_safe_range(last_diff) &&
           day2_part2_is_diff_in_safe_range(current_diff) &&
           !yo_i32_opposite_sign(current_diff, last_diff);
}

static Result day2_part2(yo_String input) {
    f64 start_time = yo_current_time_in_seconds();

    Day2Report reports[1000];
    usize      report_count = day2_parse_input(input, reports, yo_count_of(reports), yo_count_of(reports[0].values));

    u64 safe_report_count = 0;
    for (usize report_idx = 0; report_idx < report_count; ++report_idx) {
        // yo_log_debug_fmt("report %zu", report_idx);
        Day2Report* report = &reports[report_idx];

        // @NOTE: We know that report->count will always be > 3.
        u32 count = report->count;

        i32  last_diff = 0;
        u32  value_idx = 0;
        bool is_safe   = false;

        // Deal with the first three values of the report.
        {
            i32 first_diff               = yo_cast(i32, report->values[1]) - yo_cast(i32, report->values[0]);
            i32 second_diff              = yo_cast(i32, report->values[2]) - yo_cast(i32, report->values[1]);
            i32 third_diff               = yo_cast(i32, report->values[3]) - yo_cast(i32, report->values[2]);
            i32 first_diff_remove_second = yo_cast(i32, report->values[2]) - yo_cast(i32, report->values[0]);
            i32 second_diff_remove_third = yo_cast(i32, report->values[3]) - yo_cast(i32, report->values[1]);

            if (day2_part2_is_diff_safe(first_diff, second_diff)) {
                // Everything is fine.
                last_diff = second_diff;
                is_safe   = true;
                value_idx = 2;
            } else {
                if (day2_part2_is_diff_safe(second_diff, third_diff)) {
                    // We can remove the first element. Continue the search starting at index 3.
                    last_diff = third_diff;
                    is_safe   = true;
                    value_idx = 3;
                    goto after_removal;
                } else if (day2_part2_is_diff_in_safe_range(first_diff_remove_second)) {
                    // We can remove the second element. Continue the search starting at index 2.
                    last_diff = first_diff_remove_second;
                    is_safe   = true;
                    value_idx = 2;
                    goto after_removal;
                } else if (day2_part2_is_diff_safe(first_diff, second_diff_remove_third)) {
                    // We can remove the third element. Continue the search starting at index 3.
                    last_diff = second_diff_remove_third;
                    is_safe   = true;
                    value_idx = 3;
                    goto after_removal;
                }
            }
        }

        while (is_safe && (value_idx < count - 1u)) {
            i32 diff = yo_cast(i32, report->values[value_idx + 1u]) - yo_cast(i32, report->values[value_idx]);
            is_safe &= day2_part2_is_diff_safe(last_diff, diff);

            if (!is_safe) {
                if (value_idx == count - 2u) {
                    is_safe = true;  // Simply remove the last value.
                    goto end_report_check;
                } else {
                    // value_idx < count - 2u
                    i32 diff_removing_current = yo_cast(i32, report->values[value_idx + 1u]) - yo_cast(i32, report->values[value_idx - 1u]);
                    i32 diff_next             = yo_cast(i32, report->values[value_idx + 2u]) - yo_cast(i32, report->values[value_idx + 1u]);
                    i32 diff_removing_next    = yo_cast(i32, report->values[value_idx + 2u]) - yo_cast(i32, report->values[value_idx]);

                    if (day2_part2_is_diff_safe(last_diff, diff_removing_next)) {
                        // Safe to remove the next value.
                        is_safe   = true;
                        last_diff = diff_removing_next;
                    } else if (day2_part2_is_diff_safe(last_diff, diff_removing_current) && day2_part2_is_diff_safe(diff_removing_current, diff_next)) {
                        // Safe to remove the current value.
                        is_safe   = true;
                        last_diff = diff_next;
                    }

                    value_idx += 2;
                    goto after_removal;
                }
            }

            last_diff = diff;
            ++value_idx;
        }

    after_removal:
        while (is_safe && (value_idx < count - 1u)) {
            i32 diff = yo_cast(i32, report->values[value_idx + 1u]) - yo_cast(i32, report->values[value_idx]);
            is_safe &= day2_part2_is_diff_safe(last_diff, diff);
            last_diff = diff;
            ++value_idx;
        }

    end_report_check:
        safe_report_count += yo_cast(u64, is_safe);
    }

    f64 end_time = yo_current_time_in_seconds();

    return (Result){
        .result       = safe_report_count,
        .elapsed_time = end_time - start_time,
    };
}
