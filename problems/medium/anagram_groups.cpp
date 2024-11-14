// Title: Anagram groups.
// Category: Medium.
//
// Description:
//
// Given an array of strings group all anagrams together into sublists. You may return the output in
// any order.
//
// An anagram is a string that contains the exact same characters as another string, but the order
// of the characters can be different.
//
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#include <array>
#include <cassert>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

using AnagramGroups = std::vector<std::vector<std::string>>;

bool is_anagram(std::string_view lhs, std::string_view rhs) noexcept {
    constexpr size_t MAX_UNIQUE_CHARS = std::numeric_limits<char>::max();

    std::array<size_t, MAX_UNIQUE_CHARS> lhs_count = {};
    std::array<size_t, MAX_UNIQUE_CHARS> rhs_count = {};

    size_t char_count = lhs.size();
    if (char_count != rhs.size()) {
        return false;
    }

    for (size_t idx = 0; idx < char_count; ++idx) {
        ++lhs_count[static_cast<size_t>(lhs[idx])];
        ++rhs_count[static_cast<size_t>(rhs[idx])];
    }

    bool are_anagrams = true;
    for (char c : lhs) {
        if (lhs_count[static_cast<size_t>(c)] != rhs_count[static_cast<size_t>(c)]) {
            are_anagrams = false;
            break;
        }
    }

    return are_anagrams;
}

AnagramGroups anagram_groups(std::vector<std::string> const& strs) {
    size_t str_count = strs.size();

    std::vector<uint8_t> already_grouped;
    already_grouped.resize(str_count, 0);

    // In the worst case scenario, all groups are unitary.
    AnagramGroups groups;
    groups.reserve(str_count);

    for (size_t start = 0; start < str_count; ++start) {
        if (already_grouped[start] != 0) {
            continue;
        }

        // Register a new group.
        groups.emplace_back();
        std::vector<std::string>& current_group = groups.back();

        std::string const& start_str = strs[start];

        // Put the start string to the group.
        current_group.emplace_back(start_str);
        already_grouped[start] = 1;

        for (size_t next = start + 1; next < str_count; ++next) {
            if (already_grouped[next] != 0) {
                continue;
            }

            std::string const& next_str = strs[next];
            if (is_anagram(start_str, next_str)) {
                current_group.emplace_back(next_str);
                already_grouped[next] = 1;
            }
        }
    }

    return groups;
}

// @NOTE: Obviously could be optimized, but is only used for testing.
bool orderless_match(AnagramGroups const& lhs, AnagramGroups const& rhs) noexcept {
    size_t group_count = lhs.size();
    if (group_count != rhs.size()) {
        return false;
    }

    bool does_match = true;
    for (size_t lhs_idx = 0; lhs_idx < group_count; ++lhs_idx) {
        bool found = false;

        auto const& lhs_group  = lhs[lhs_idx];
        size_t      group_size = lhs_group.size();

        for (size_t rhs_idx = 0; rhs_idx < group_count; ++rhs_idx) {
            auto const& rhs_group = lhs[lhs_idx];

            if (rhs_group.size() != group_size) {
                continue;
            }

            size_t match_count = 0;
            for (auto const& lhs_str : lhs_group) {
                for (auto const& rhs_str : rhs_group) {
                    match_count += static_cast<size_t>(lhs_str == rhs_str);
                }
            }

            found = (match_count == group_size);
            if (found) {
                break;
            }
        }

        // Ensure all groups should have a match.
        does_match &= found;

        // If any group doesn't have a match, then the anagram groups differ.
        if (!does_match) {
            break;
        }
    }

    return does_match;
}

int main(int, char** argv) {
    // @NOTE: Using vector is obviously not needed but these exercises are completely stupid anyway...

    // Test 1.
    {
        std::vector<std::string> strs = {"act", "pots", "tops", "cat", "stop", "hat"};

        AnagramGroups expect = {{"hat"}, {"act", "cat"}, {"stop", "pots", "tops"}};
        AnagramGroups groups = anagram_groups(strs);
        assert(orderless_match(expect, groups));
    }

    // Test 2.
    {
        std::vector<std::string> strs = {"x"};

        AnagramGroups expect = {{"x"}};
        AnagramGroups groups = anagram_groups(strs);
        assert(orderless_match(expect, groups));
    }

    // Test 3.
    {
        std::vector<std::string> strs = {""};

        AnagramGroups expect = {{""}};
        AnagramGroups groups = anagram_groups(strs);
        assert(orderless_match(expect, groups));
    }

    std::printf("%s: Success", argv[0]);
    return 0;
}
