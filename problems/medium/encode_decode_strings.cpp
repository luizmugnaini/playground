// ------------------------------------------------------------------------------------------------
// Title: Encode and decode strings.
// Category: Medium.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// Design an algorithm to encode a list of strings to a single string. The encoded string is then
// decoded back to the original list of strings.
// ------------------------------------------------------------------------------------------------

#include <string>
#include <vector>
#include "../common.hpp"

constexpr char SEPARATOR = '#';

size_t count_digits(size_t value) {
    size_t count = 0;
    while (value != 0) {
        value /= 10;
        ++count;
    }
    return count;
}

std::string encode(std::vector<std::string> const& strs) {
    size_t encoding_length = 0;
    for (auto const& str : strs) {
        size_t length = str.length();
        encoding_length += count_digits(length) + 1 + length;
    }

    std::string encoding;
    encoding.reserve(encoding_length);

    for (auto const& str : strs) {
        encoding.append(std::to_string(str.length()) + SEPARATOR + str);
    }

    return encoding;
}

std::vector<std::string> decode(std::string const& encoded) {
    std::vector<std::string> decoded;

    size_t encoding_length = encoded.length();

    size_t length_start_idx = 0;
    for (size_t idx = 0; idx < encoding_length;) {
        if (encoded[idx] == SEPARATOR) {
            size_t str_length = std::stoull(encoded.c_str() + length_start_idx);

            decoded.emplace_back(encoded.substr(idx + 1, str_length));

            length_start_idx = idx + str_length + 1;
            idx += str_length + 2;
        } else {
            ++idx;
        }
    }

    return decoded;
}

bool ordered_match(std::vector<std::string> const& lhs, std::vector<std::string> const& rhs) {
    size_t count = lhs.size();
    if (rhs.size() != count) {
        return false;
    }

    bool matches = true;
    for (size_t idx = 0; idx < count; ++idx) {
        matches &= (lhs[idx] == rhs[idx]);
    }

    return matches;
}

void dbg(std::string const& enc, std::vector<std::string> const& dec) {
    std::cout << "encoded: " << enc << "\n";
    std::cout << "decoded: [";
    for (auto const& s : dec) {
        std::cout << s << ", ";
    }
    std::cout << "]\n";
}

int main() {
    // Test 1.
    {
        std::vector<std::string> strs = {"One", "ring", "to", "rule", "them", "all"};

        auto encoded = encode(strs);
        auto decoded = decode(encoded);
        assert(ordered_match(strs, decoded));
    }

    // Test 2.
    {
        std::vector<std::string> strs = {"Earendil", "was", "a", "mariner"};

        auto encoded = encode(strs);
        auto decoded = decode(encoded);
        assert(ordered_match(strs, decoded));
    }

    // Test 3.
    {
        std::vector<std::string> strs = {"Nine", "for", "the", "Elven-kings", "under", "moon", "and", "star"};

        auto encoded = encode(strs);
        auto decoded = decode(encoded);
        assert(ordered_match(strs, decoded));
    }

    // Test 4.
    {
        std::vector<std::string> strs = {"##%1928372", "#18237#", "12#182#73##"};

        auto encoded = encode(strs);
        auto decoded = decode(encoded);
        assert(ordered_match(strs, decoded));
    }

    report_success();

    return 0;
}
