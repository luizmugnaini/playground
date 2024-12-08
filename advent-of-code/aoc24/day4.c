#include <string.h>
#include "common.h"

#define DAY4_INPUT_MATRIX_COLUMN_COUNT 10                               // 140
#define DAY4_INPUT_MATRIX_ROW_COUNT    10                               // 140
#if defined(YO_OS_WINDOWS)
#    define DAY4_INPUT_LINE_WIDTH (DAY4_INPUT_MATRIX_COLUMN_COUNT + 2)  // plus crlf
#else
#    define DAY4_INPUT_LINE_WIDTH (DAY4_INPUT_MATRIX_COLUMN_COUNT + 1)  // plus lf
#endif

#define day4_input_line_start(input, line_idx) (input.buf + line_idx * DAY4_INPUT_LINE_WIDTH)

//                           S..S..S
//                           .A.A.A.
//                           ..MMM..
// A window is of the form:  SAMXMAS
//                           ..MMM..
//                           .A.A.A.
//                           S..S..S
#define DAY4_WINDOW_DIMENSION 7

typedef struct Window {
    char buf[DAY4_WINDOW_DIMENSION * DAY4_WINDOW_DIMENSION];
} Window;

#define day4_copy_whole_window(window, center_pos)                                               \
    do {                                                                                         \
        u8 const* start_pos_ = yo_cast(u8 const*, center_pos - (3 * DAY4_INPUT_LINE_WIDTH + 3)); \
        for (u32 row_ = 0; row_ < 7; ++row_) {                                                   \
            memcpy(                                                                              \
                yo_cast(u8*, window.buf + row_ * DAY4_WINDOW_DIMENSION),                         \
                yo_cast(u8 const*, start_pos_ + row_ * DAY4_INPUT_LINE_WIDTH),                   \
                yo_size_of(window.buf));                                                         \
        }                                                                                        \
    } while (0)

// Fetch substrings bounded by a given window.
#define day4_window_get_xmas_west(xmas_buf, window)                     \
    do {                                                                \
        memcpy(                                                         \
            yo_cast(u8*, xmas_buf),                                     \
            yo_cast(u8 const*, window.buf + 3 * DAY4_WINDOW_DIMENSION), \
            yo_size_of(xmas_buf));                                      \
    } while (0)
#define day4_window_get_xmas_east(xmas_buf, center)                         \
    do {                                                                    \
        memcpy(                                                             \
            yo_cast(u8*, xmas_buf),                                         \
            yo_cast(u8 const*, window.buf + 3 * DAY4_WINDOW_DIMENSION + 3), \
            yo_size_of(xmas_buf));                                          \
    } while (0)
#define day4_window_get_xmas_north(xmas_buf, window)             \
    do {                                                         \
        xmas_buf[0] = window.buf[3 * DAY4_WINDOW_DIMENSION + 3]; \
        xmas_buf[1] = window.buf[2 * DAY4_WINDOW_DIMENSION + 3]; \
        xmas_buf[2] = window.buf[DAY4_WINDOW_DIMENSION + 3];     \
        xmas_buf[3] = window.buf[3];                             \
    } while (0)
#define day4_window_get_xmas_northwest(xmas_buf, window)         \
    do {                                                         \
        xmas_buf[0] = window.buf[3 * DAY4_WINDOW_DIMENSION + 3]; \
        xmas_buf[1] = window.buf[2 * DAY4_WINDOW_DIMENSION + 2]; \
        xmas_buf[2] = window.buf[DAY4_WINDOW_DIMENSION + 1];     \
        xmas_buf[3] = window.buf[0];                             \
    } while (0)
#define day4_window_get_xmas_northeast(xmas_buf, window)         \
    do {                                                         \
        xmas_buf[0] = window.buf[3 * DAY4_WINDOW_DIMENSION + 3]; \
        xmas_buf[1] = window.buf[2 * DAY4_WINDOW_DIMENSION + 4]; \
        xmas_buf[2] = window.buf[DAY4_WINDOW_DIMENSION + 5];     \
        xmas_buf[3] = window.buf[6];                             \
    } while (0)
#define day4_window_get_xmas_south(xmas_buf, window)             \
    do {                                                         \
        xmas_buf[0] = window.buf[3 * DAY4_WINDOW_DIMENSION + 3]; \
        xmas_buf[1] = window.buf[4 * DAY4_WINDOW_DIMENSION + 3]; \
        xmas_buf[2] = window.buf[5 * DAY4_WINDOW_DIMENSION + 3]; \
        xmas_buf[3] = window.buf[6 * DAY4_WINDOW_DIMENSION + 3]; \
    } while (0)
#define day4_window_get_xmas_southwest(xmas_buf, window)         \
    do {                                                         \
        xmas_buf[0] = window.buf[3 * DAY4_WINDOW_DIMENSION + 3]; \
        xmas_buf[1] = window.buf[4 * DAY4_WINDOW_DIMENSION + 2]; \
        xmas_buf[2] = window.buf[5 * DAY4_WINDOW_DIMENSION + 1]; \
        xmas_buf[3] = window.buf[6 * DAY4_WINDOW_DIMENSION];     \
    } while (0)
#define day4_window_get_xmas_southeast(xmas_buf, window)         \
    do {                                                         \
        xmas_buf[0] = window.buf[3 * DAY4_WINDOW_DIMENSION + 3]; \
        xmas_buf[1] = window.buf[4 * DAY4_WINDOW_DIMENSION + 4]; \
        xmas_buf[2] = window.buf[5 * DAY4_WINDOW_DIMENSION + 5]; \
        xmas_buf[3] = window.buf[6 * DAY4_WINDOW_DIMENSION + 6]; \
    } while (0)

// Fetch substrings directly from a position from the input string.
#define day4_input_get_xmas_west(buf, input_pos)                                                            \
    do {                                                                                                    \
        memcpy(yo_cast(u8*, buf), yo_cast(u8 const*, input_pos - (yo_count_of(buf) - 1)), yo_size_of(buf)); \
    } while (0)
#define day4_input_get_xmas_east(buf, input_pos)                                   \
    do {                                                                           \
        memcpy(yo_cast(u8*, buf), yo_cast(u8 const*, input_pos), yo_size_of(buf)); \
    } while (0)
#define day4_input_get_xmas_north(buf, input_pos)          \
    do {                                                   \
        buf[0] = *input_pos;                               \
        buf[1] = *(input_pos - DAY4_INPUT_LINE_WIDTH);     \
        buf[2] = *(input_pos - 2 * DAY4_INPUT_LINE_WIDTH); \
        buf[3] = *(input_pos - 3 * DAY4_INPUT_LINE_WIDTH); \
    } while (0)
#define day4_input_get_xmas_northwest(buf, input_pos)          \
    do {                                                       \
        buf[0] = *input_pos;                                   \
        buf[1] = *(input_pos - DAY4_INPUT_LINE_WIDTH - 1);     \
        buf[2] = *(input_pos - 2 * DAY4_INPUT_LINE_WIDTH - 2); \
        buf[3] = *(input_pos - 3 * DAY4_INPUT_LINE_WIDTH - 3); \
    } while (0)
#define day4_input_get_xmas_northeast(buf, input_pos)          \
    do {                                                       \
        buf[0] = *input_pos;                                   \
        buf[1] = *(input_pos + DAY4_INPUT_LINE_WIDTH + 1);     \
        buf[2] = *(input_pos + 2 * DAY4_INPUT_LINE_WIDTH + 2); \
        buf[3] = *(input_pos + 3 * DAY4_INPUT_LINE_WIDTH + 3); \
    } while (0)
#define day4_input_get_xmas_south(buf, input_pos)          \
    do {                                                   \
        buf[0] = *input_pos;                               \
        buf[1] = *(input_pos + DAY4_INPUT_LINE_WIDTH);     \
        buf[2] = *(input_pos + 2 * DAY4_INPUT_LINE_WIDTH); \
        buf[3] = *(input_pos + 3 * DAY4_INPUT_LINE_WIDTH); \
    } while (0)
#define day4_input_get_xmas_southwest(buf, input_pos)          \
    do {                                                       \
        buf[0] = *input_pos;                                   \
        buf[1] = *(input_pos + DAY4_INPUT_LINE_WIDTH - 1);     \
        buf[2] = *(input_pos + 2 * DAY4_INPUT_LINE_WIDTH - 2); \
        buf[3] = *(input_pos + 3 * DAY4_INPUT_LINE_WIDTH - 3); \
    } while (0)
#define day4_input_get_xmas_southeast(buf, input_pos)          \
    do {                                                       \
        buf[0] = *input_pos;                                   \
        buf[1] = *(input_pos + DAY4_INPUT_LINE_WIDTH + 1);     \
        buf[2] = *(input_pos + 2 * DAY4_INPUT_LINE_WIDTH + 2); \
        buf[3] = *(input_pos + 3 * DAY4_INPUT_LINE_WIDTH + 3); \
    } while (0)

static Result day4_part1(yo_String input) {
    f64 start_time = yo_current_time_in_seconds();

    yo_String xmas_string = yo_comptime_make_string("XMAS");
    u64       xmas_count  = 0;

    // Sweep the first three lines, those we can't build an entire window.
    for (u32 line_idx = 0; line_idx < 3; ++line_idx) {
        char const* current_pos = day4_input_line_start(input, line_idx);

        char xmas_buf[4];

        // Process the first 3 columns. Those can form south, east, and southeast.
        for (u32 col_idx = 0; col_idx < 3; ++col_idx) {
            day4_input_get_xmas_east(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_south(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_southeast(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            ++current_pos;
        }

        // Process the middle columns. Those can form west, east, south, southwest, and southeast.
        for (u32 col_idx = 3; col_idx < DAY4_INPUT_MATRIX_COLUMN_COUNT - 3; ++col_idx) {
            day4_input_get_xmas_west(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_east(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_south(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_southeast(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            ++current_pos;
        }

        // Process the last 3 columns. Those can form south, west, and southwest.
        for (u32 col_idx = DAY4_INPUT_MATRIX_COLUMN_COUNT - 3; col_idx < DAY4_INPUT_MATRIX_COLUMN_COUNT; ++col_idx) {
            day4_input_get_xmas_west(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_south(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_southwest(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            ++current_pos;
        }
    }

    // Process the middle lines.
    for (u32 line_idx = 3; line_idx < DAY4_INPUT_MATRIX_ROW_COUNT - 3; ++line_idx) {
        char const* current_pos = day4_input_line_start(input, line_idx);

        char xmas_buf[4];

        // Process the first 3 columns. Those can form south, north, east, southeast, and northeast.
        for (u32 col_idx = 0; col_idx < 3; ++col_idx) {
            day4_input_get_xmas_east(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_north(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_northeast(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_south(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_southeast(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            ++current_pos;
        }

        // Process the middle columns. Those can form all of the directions.
        {
            Window window;
            day4_copy_whole_window(window, current_pos);

            for (u32 col_idx = 3; col_idx < DAY4_INPUT_MATRIX_COLUMN_COUNT - 3; ++col_idx) {
                day4_window_get_xmas_west(xmas_buf, window);
                xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

                day4_window_get_xmas_east(xmas_buf, window);
                xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

                day4_window_get_xmas_northwest(xmas_buf, window);
                xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

                day4_window_get_xmas_north(xmas_buf, window);
                xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

                day4_window_get_xmas_northeast(xmas_buf, window);
                xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

                day4_window_get_xmas_southwest(xmas_buf, window);
                xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

                day4_window_get_xmas_south(xmas_buf, window);
                xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

                day4_window_get_xmas_southeast(xmas_buf, window);
                xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

                ++current_pos;
            }
        }

        // Process the last 3 columns. Those can form south, southwest, north, northwest, and west.
        for (u32 col_idx = DAY4_INPUT_MATRIX_COLUMN_COUNT - 3; col_idx < DAY4_INPUT_MATRIX_COLUMN_COUNT; ++col_idx) {
            day4_input_get_xmas_west(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_north(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_south(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_southwest(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            ++current_pos;
        }
    }

    // Process the last three lines, those we can't build an entire window.
    for (u32 line_idx = DAY4_INPUT_MATRIX_ROW_COUNT - 3; line_idx < DAY4_INPUT_MATRIX_ROW_COUNT; ++line_idx) {
        char const* current_pos = day4_input_line_start(input, line_idx);

        char xmas_buf[4];

        // Process the first 3 columns. Those can form north, east, and northeast.
        for (u32 col_idx = 0; col_idx < 3; ++col_idx) {
            day4_input_get_xmas_east(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_north(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_northeast(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            ++current_pos;
        }

        // Process the middle columns. Those can form west, east, north, northwest, and northeast.
        for (u32 col_idx = 3; col_idx < DAY4_INPUT_MATRIX_COLUMN_COUNT - 3; ++col_idx) {
            day4_input_get_xmas_west(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_east(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_north(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_northwest(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_northeast(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            ++current_pos;
        }

        // Process the last 3 columns. Those can form south, west, and southwest.
        for (u32 col_idx = DAY4_INPUT_MATRIX_COLUMN_COUNT - 3; col_idx < DAY4_INPUT_MATRIX_COLUMN_COUNT; ++col_idx) {
            day4_input_get_xmas_west(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_south(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            day4_input_get_xmas_southwest(xmas_buf, current_pos);
            xmas_count += yo_cast(u64, yo_string_equal(yo_comptime_make_string_from_array(xmas_buf), xmas_string));

            ++current_pos;
        }
    }

    f64 end_time = yo_current_time_in_seconds();

    return (Result){
        .result       = xmas_count,
        .elapsed_time = end_time - start_time,
    };
}
