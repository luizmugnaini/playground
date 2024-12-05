#include <yoneda_core.h>

typedef struct Day3Result {
    u64 result;
    f64 elapsed_time;
} Day3Result;

static Day3Result day3_part1(yo_String input) {
    f64 start_time = yo_current_time_in_seconds();

    u64 result = 0;

    yo_String mul_token = yo_comptime_make_string("mul");

    char const* input_end   = input.buf + input.length;
    char const* current_pos = input.buf;
    while (current_pos < input_end) {
        // If we aren't in a mul expression candidate, just continue;
        if ((*current_pos != 'm') || !yo_string_equal((yo_String){current_pos, mul_token.length}, mul_token)) {
            ++current_pos;
            continue;
        }

        // Since we found a "mul", advance to the next token.
        current_pos += mul_token.length;

        // Tracks the validity of the expression through the parsing.
        bool is_valid_expression = true;

        // The numbers are ensured to be up to 3 digits long.
        usize number_length = 0;
        char  number_buf[4] = {0};

        // Check if we are at the opening of an expression, that is, "mul("
        //                                                               ^
        is_valid_expression &= (current_pos < input_end) && (*current_pos == '(');

        // Parse the left-hand-side value, that is, "mul(X"
        //                                               ^
        u32 lhs = 0;
        if (is_valid_expression) {
            // Skip '('.
            ++current_pos;

            while (current_pos < input_end - 1) {
                char c = *current_pos;
                if (!yo_char_is_digit(c)) {
                    break;
                }

                number_buf[number_length++] = c;
                ++current_pos;
            }

            is_valid_expression &= yo_string_to_u32((yo_String){number_buf, number_length}, &lhs);
            number_length = 0;
        }

        // Check if we got a comma, that is, "mul(X,"
        //                                         ^
        is_valid_expression &= (current_pos < input_end) && (*current_pos == ',');

        // Parse the right-hand-side value, that is, "mul(X,Y"
        //                                                  ^
        u32 rhs = 0;
        if (is_valid_expression) {
            // Skip ','.
            ++current_pos;

            while (current_pos < input_end - 1) {
                char c = *current_pos;
                if (!(yo_char_is_digit(c) && (number_length < yo_size_of(number_buf)))) {
                    break;
                }

                number_buf[number_length++] = c;
                ++current_pos;
            }

            is_valid_expression &= yo_string_to_u32((yo_String){number_buf, number_length}, &rhs);
        }

        // Check if we got a closing of an expression (i.e. "mul(X,Y)").
        //                                                          ^
        is_valid_expression &= (current_pos < input_end) && (*current_pos == ')');

        // If everything went ok, accumulate the multiplication into the result.
        if (is_valid_expression) {
            // Skip ')'.
            ++current_pos;

            result += yo_cast(u64, rhs) * yo_cast(u64, lhs);
        }
    }

    f64 end_time = yo_current_time_in_seconds();

    return (Day3Result){
        .result       = result,
        .elapsed_time = end_time - start_time,
    };
}

static Day3Result day3_part2(yo_String input) {
    f64 start_time = yo_current_time_in_seconds();

    u64 result = 0;

    yo_String do_instruction   = yo_comptime_make_string("do()");
    yo_String dont_instruction = yo_comptime_make_string("don't()");
    yo_String mul_token        = yo_comptime_make_string("mul");

    bool mul_enabled = true;

    char const* input_end   = input.buf + input.length;
    char const* current_pos = input.buf;
    while (current_pos < input_end) {
        // Check if we have a do() or don't() instruction.
        if (*current_pos == 'd') {
            usize offset = 1;

            if (yo_string_equal((yo_String){current_pos, do_instruction.length}, do_instruction)) {
                mul_enabled = true;
                offset      = do_instruction.length;
            } else if (yo_string_equal((yo_String){current_pos, dont_instruction.length}, dont_instruction)) {
                mul_enabled = false;
                offset      = dont_instruction.length;
            }

            current_pos += offset;
            continue;
        }

        // If we aren't in a mul expression candidate, just continue;
        if (!mul_enabled || (*current_pos != 'm') || !yo_string_equal((yo_String){current_pos, mul_token.length}, mul_token)) {
            ++current_pos;
            continue;
        }

        // Since we found a "mul", advance to the next token.
        current_pos += mul_token.length;

        // Tracks the validity of the expression through the parsing.
        bool is_valid_expression = true;

        // The numbers are ensured to be up to 3 digits long.
        usize number_length = 0;
        char  number_buf[4] = {0};

        // Check if we are at the opening of an expression, that is, "mul("
        //                                                               ^
        is_valid_expression &= (current_pos < input_end) && (*current_pos == '(');

        // Parse the left-hand-side value, that is, "mul(X"
        //                                               ^
        u32 lhs = 0;
        if (is_valid_expression) {
            // Skip '('.
            ++current_pos;

            while (current_pos < input_end - 1) {
                char c = *current_pos;
                if (!yo_char_is_digit(c)) {
                    break;
                }

                number_buf[number_length++] = c;
                ++current_pos;
            }

            is_valid_expression &= yo_string_to_u32((yo_String){number_buf, number_length}, &lhs);
            number_length = 0;
        }

        // Check if we got a comma, that is, "mul(X,"
        //                                         ^
        is_valid_expression &= (current_pos < input_end) && (*current_pos == ',');

        // Parse the right-hand-side value, that is, "mul(X,Y"
        //                                                  ^
        u32 rhs = 0;
        if (is_valid_expression) {
            // Skip ','.
            ++current_pos;

            while (current_pos < input_end - 1) {
                char c = *current_pos;
                if (!(yo_char_is_digit(c) && (number_length < yo_size_of(number_buf)))) {
                    break;
                }

                number_buf[number_length++] = c;
                ++current_pos;
            }

            is_valid_expression &= yo_string_to_u32((yo_String){number_buf, number_length}, &rhs);
        }

        // Check if we got a closing of an expression (i.e. "mul(X,Y)").
        //                                                          ^
        is_valid_expression &= (current_pos < input_end) && (*current_pos == ')');

        // If everything went ok, accumulate the multiplication into the result.
        if (is_valid_expression) {
            // Skip ')'.
            ++current_pos;

            result += yo_cast(u64, rhs) * yo_cast(u64, lhs);
        }
    }

    f64 end_time = yo_current_time_in_seconds();

    return (Day3Result){
        .result       = result,
        .elapsed_time = end_time - start_time,
    };
}
