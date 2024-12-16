// ------------------------------------------------------------------------------------------------
// Title: Task scheduler.
// Category: Medium.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// You are given an array of CPU tasks tasks, where tasks[i] is an uppercase english character
// from A to Z. You are also given an integer n.
//
// Each CPU cycle allows the completion of a single task, and tasks may be completed in any order.
//
// The only constraint is that identical tasks must be separated by at least n CPU cycles, to
// cooldown the CPU.
//
// Return the minimum number of CPU cycles required to complete all tasks.
// ------------------------------------------------------------------------------------------------

#include <common.hpp>

static constexpr uint32_t TASK_TYPES_COUNT = 'Z' - 'A' + 1;

static uint32_t min_cpu_cycles_to_completion(
    std::vector<char> const& tasks,
    uint32_t                 separation_cycles_of_identical_tasks) {
    uint32_t task_slots[TASK_TYPES_COUNT] = {};

    for (char task : tasks) {
        ++task_slots[task - 'A'];
    }

    uint32_t count_of_task_with_max_count       = 0;
    int32_t  count_of_task_types_with_max_count = 0;
    for (size_t slot = 0; slot < TASK_TYPES_COUNT; ++slot) {
        uint32_t current_task_count = task_slots[slot];
        if (count_of_task_with_max_count < current_task_count) {
            count_of_task_with_max_count       = current_task_count;
            count_of_task_types_with_max_count = 1;
        } else if (current_task_count == count_of_task_with_max_count) {
            ++count_of_task_types_with_max_count;
        }
    }

    // Time needed to fit the separation slots. The element with highest count will dictate the cycle
    // periods and the remaining tasks will be put inside of these separation cycles. There will be
    // count_of_task_types_with_max_count - 1 periods, whose length will be of separation_cycles_of_identical_tasks + 1
    // cycles (the task + separation time).
    //
    // Ex: Take {A, B, B, B, C, D, D} with a separation of 3 cycles, we'll have the following scheduling:
    //
    //                                       B D A C B D * * B
    //                                       |     | |     | ^
    //                                       |--x--| |--x--|
    //
    //     Notice how there are two periods (B count - 1), with B being the leading task. Also, x is
    //     the length of the period (including B, i.e. separation_cycles_of_identical_tasks + 1).
    //
    //     Notice that this isn't sufficient to account for all tasks - we've let the last B out of
    //     the schedule. There could exist other task types that wouldn't fit the last period. The
    //     next example will account for that.
    //
    //
    // After the last period of the task with highest count, there will be appended all of the tasks
    // with the same max count.
    //
    // Ex: If we have {A, A, B, B, C, C, D, D} with a separation of 4 cycles, we'll have the following
    //     scheduling:
    //
    //                                        A B C D * A B C D
    //                                        |       | ^ ^ ^ ^
    //                                        |---x---|
    //
    //     In this example we can see that A is the leading task and B, C, and D all have the same count
    //     as A - hence we have to append them to the end of every A period (marked with ^).
    //
    //
    // We have yet another case to consider: what if the tasks with maximum count occupy the entire period
    // of separation? There won't be space left for the the remaining tasks and they will need to be added
    // to the end of the sequence. In this case, all cycles are spent executing a task (no idle state). Thus
    // the necessary number of cycles to complete the batch is simply the number of tasks handled to the CPU.
    // We can detect if this case has occurred whenever the cycles_to_completion_with_separation_as_period
    // doesn't account for, at least, all tasks of the batch.
    //
    // Ex: If we have {A, A, B, B, C, D} with a separation of 1 cycles, we'll have the following
    //
    //                                       A B A B C D
    //                                       | | ^ ^ & &
    //                                       |x|
    //
    //     Here we have marked with & the tasks that would be left to crumble if we tried to fit them into the
    //     scheduling strategy we developed above, since both A and B fill the entire separation period. In this
    //     case, the total number of cycles should be equal to the total number of tasks.
    int32_t cycles_to_completion_with_separation_as_period =
        (count_of_task_with_max_count - 1u) * (separation_cycles_of_identical_tasks + 1u) + count_of_task_types_with_max_count;

    return std::max(
        static_cast<uint32_t>(cycles_to_completion_with_separation_as_period),
        static_cast<uint32_t>(tasks.size()));
}

int main() {
    assert_eq(min_cpu_cycles_to_completion({'X', 'X', 'Y', 'Y'}, 1), 4);
    assert_eq(min_cpu_cycles_to_completion({'X', 'X', 'Y', 'Y'}, 2), 5);
    assert_eq(min_cpu_cycles_to_completion({'A', 'A', 'B', 'B', 'C', 'C'}, 2), 6);
    assert_eq(min_cpu_cycles_to_completion({'A', 'A', 'A', 'B', 'C'}, 3), 9);
    assert_eq(min_cpu_cycles_to_completion({'A', 'A', 'B', 'B', 'C', 'C', 'D', 'D'}, 4), 9);
    assert_eq(min_cpu_cycles_to_completion({'A', 'B', 'B', 'B', 'C', 'D', 'D'}, 3), 9);
    assert_eq(min_cpu_cycles_to_completion({'A', 'A', 'B', 'B', 'C', 'D'}, 1), 6);
    report_success();
    return 0;
}
