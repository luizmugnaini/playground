// clang-format off
#include "common.c"
#include "day1.c"
// clang-format on

#include <yoneda_log.h>
#include <yoneda_memory.h>
#include <yoneda_time.h>

int main(void) {
    yo_Arena arena = yo_make_owned_arena(yo_mebibytes(4));

    // Day 1.
    {
        yo_ArenaCheckpoint arena_checkpoint = yo_make_arena_checkpoint(&arena);

        f64 start_time1  = yo_current_time_in_seconds();
        u64 part1_result = day1_part1(&arena);
        f64 end_time1    = yo_current_time_in_seconds();
        yo_log_info_fmt("DAY 1, PART 1 (%f seconds): %zu.", end_time1 - start_time1, part1_result);

        f64 start_time2  = yo_current_time_in_seconds();
        u64 part2_result = day1_part2(&arena);
        f64 end_time2    = yo_current_time_in_seconds();
        yo_log_info_fmt("DAY 1, PART 2 (%f seconds): %zu.", end_time2 - start_time2, part2_result);

        yo_arena_checkpoint_restore(arena_checkpoint);
    }

    yo_free_owned_arena(&arena);
    return 0;
}
