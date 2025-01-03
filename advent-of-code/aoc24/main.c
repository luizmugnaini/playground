// clang-format off
#include "common.c"
#include "day1.c"
#include "day2.c"
#include "day3.c"
#include "day4.c"
// clang-format on

#include <yoneda_log.h>
#include <yoneda_memory.h>
#include <yoneda_time.h>

static yo_String get_input(yo_Arena* arena, cstring file_path) {
    yo_FileReadResult read_result = yo_read_file(arena, file_path, YO_FILE_FLAG_READ_BIN);
    yo_assert(read_result.status == YO_FILE_STATUS_NONE);
    return (yo_String){
        .buf    = yo_cast(char*, read_result.buf),
        .length = read_result.buf_size * yo_size_of(char),
    };
}

int main(void) {
    yo_Arena arena = yo_make_owned_arena(yo_mebibytes(4));

    // Day 1.
    {
        yo_ArenaCheckpoint arena_checkpoint = yo_make_arena_checkpoint(&arena);

        Result part1 = day1_part1(get_input(&arena, make_input_path("1", "1")));
        Result part2 = day1_part2(get_input(&arena, make_input_path("1", "2")));

        yo_log_info_fmt("DAY 1, PART 1 (%.9f seconds): %zu.", part1.elapsed_time, part1.result);
        yo_log_info_fmt("DAY 1, PART 2 (%.9f seconds): %zu.", part2.elapsed_time, part2.result);

        yo_arena_checkpoint_restore(arena_checkpoint);
    }

    // Day 2.
    {
        yo_ArenaCheckpoint arena_checkpoint = yo_make_arena_checkpoint(&arena);

        Result part1 = day2_part1(get_input(&arena, make_input_path("2", "1")));
        Result part2 = day2_part2(get_input(&arena, make_input_path("2", "2")));

        yo_log_info_fmt("DAY 2, PART 1 (%.9f seconds): %zu.", part1.elapsed_time, part1.result);
        yo_log_info_fmt("DAY 2, PART 2 (%.9f seconds): %zu.", part2.elapsed_time, part2.result);

        yo_arena_checkpoint_restore(arena_checkpoint);
    }

    // Day 3.
    {
        yo_ArenaCheckpoint arena_checkpoint = yo_make_arena_checkpoint(&arena);

        Result part1 = day3_part1(get_input(&arena, make_input_path("3", "1")));
        Result part2 = day3_part2(get_input(&arena, make_input_path("3", "2")));

        yo_log_info_fmt("DAY 3, PART 1 (%.9f seconds): %zu.", part1.elapsed_time, part1.result);
        yo_log_info_fmt("DAY 3, PART 2 (%.9f seconds): %zu.", part2.elapsed_time, part2.result);

        yo_arena_checkpoint_restore(arena_checkpoint);
    }

    // Day 4.
    {
        yo_ArenaCheckpoint arena_checkpoint = yo_make_arena_checkpoint(&arena);

        Result part1 = day4_part1(get_input(
            &arena,
            "aoc24/input/day4_part1_sample.txt"));
        // make_input_path("4", "1")));
        // Result part2 = day4_part2(get_input(&arena, make_input_path("4", "2")));

        yo_log_info_fmt("DAY 4, PART 1 (%.9f seconds): %zu.", part1.elapsed_time, part1.result);
        // yo_log_info_fmt("DAY 4, PART 2 (%.9f seconds): %zu.", part2.elapsed_time, part2.result);

        yo_arena_checkpoint_restore(arena_checkpoint);
    }

    yo_free_owned_arena(&arena);
    return 0;
}
