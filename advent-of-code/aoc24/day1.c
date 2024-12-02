#include <yoneda_core.h>
#include <yoneda_log.h>
#include <yoneda_math.h>
#include <yoneda_memory.h>
#include <yoneda_streams.h>
#include <yoneda_time.h>

typedef struct Day1Result {
    u64 result;
    f64 elapsed_time;
} Day1Result;

static u64 day1_parse_input_file(yo_String input, u32* ids_left, u32* ids_right, usize id_max_count) {
    char const* input_end = yo_cast(char const*, input.buf + input.length);

    usize const NUMBER_LENGTH = 5;  // @NOTE: Very, very hard coded, we only need perf.
    char        number_buf[8] = {0};

    u64 id_count = 0;

    char const* current_char = yo_cast(char const*, input.buf);
    while ((current_char < input_end) && (id_count <= id_max_count)) {
        // Parse ID 1.
        yo_memory_move(yo_cast(u8*, number_buf), yo_cast(u8 const*, current_char), NUMBER_LENGTH);
        yo_assert(yo_string_to_u32((yo_String){number_buf, NUMBER_LENGTH}, &ids_left[id_count]));

        current_char += NUMBER_LENGTH + 3;  // Skip three spaces and go to next ID.

        // Parse ID 2.
        yo_memory_move(yo_cast(u8*, number_buf), yo_cast(u8 const*, current_char), NUMBER_LENGTH);
        yo_assert_fmt(yo_string_to_u32((yo_String){number_buf, NUMBER_LENGTH}, &ids_right[id_count]), "%s", number_buf);

        current_char += NUMBER_LENGTH;

        // Skip to the next line.
        while (yo_char_is_crlf(*current_char)) {
            ++current_char;
        }

        ++id_count;
    }

    return id_count;
}

static Day1Result day1_part1(yo_String input) {
    f64 start_time = yo_current_time_in_seconds();

    u32   ids_left[1000];
    u32   ids_right[1000];
    usize id_count = day1_parse_input_file(input, ids_left, ids_right, yo_count_of(ids_left));

    quick_sort_u32_range(ids_left, ids_left + id_count - 1);
    quick_sort_u32_range(ids_right, ids_right + id_count - 1);

    i64 distance = 0;
    for (usize idx = 0; idx < id_count; ++idx) {
        distance += yo_i32_abs_value(yo_cast(i32, ids_left[idx]) - yo_cast(i32, ids_right[idx]));
    }

    f64 end_time = yo_current_time_in_seconds();

    return (Day1Result){
        .result       = yo_cast(u64, distance),
        .elapsed_time = end_time - start_time,
    };
}

static Day1Result day1_part2(yo_String input) {
    f64 start_time = yo_current_time_in_seconds();

    u32   ids_left[1000];
    u32   ids_right[1000];
    usize id_count = day1_parse_input_file(input, ids_left, ids_right, yo_count_of(ids_left));

    quick_sort_u32_range(ids_left, ids_left + id_count - 1);
    quick_sort_u32_range(ids_right, ids_right + id_count - 1);

    u64 similarity_score = 0;
    for (usize idx = 0; idx < id_count; ++idx) {
        u32   left_value = ids_left[idx];
        isize right_idx  = binary_search_u32_values(ids_right, id_count, left_value);

        if (right_idx < 0) {
            continue;
        }

        // Find first occurrence.
        isize first_idx = right_idx;
        while ((first_idx > 0) && (ids_right[first_idx - 1] == left_value)) {
            --first_idx;
        }

        usize appearence_count = yo_cast(usize, right_idx - first_idx) + 1u;

        // Count to the right of the binary search index result.
        isize last_idx = right_idx + 1;
        while ((last_idx < yo_cast(isize, id_count)) && (ids_right[last_idx] == left_value)) {
            ++appearence_count;
            ++last_idx;
        }

        similarity_score += left_value * appearence_count;
    }

    f64 end_time = yo_current_time_in_seconds();

    return (Day1Result){
        .result       = similarity_score,
        .elapsed_time = end_time - start_time,
    };
}
