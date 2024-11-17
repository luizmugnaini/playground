// ------------------------------------------------------------------------------------------------
// Title: Valid sudoku board.
// Category: Medium.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// You are given a a 9 x 9 Sudoku board board. A Sudoku board is valid if the following rules are followed:
// 1. Each row must contain the digits 1-9 without duplicates.
// 2. Each column must contain the digits 1-9 without duplicates.
// 3. Each of the nine 3 x 3 sub-boxes of the grid must contain the digits 1-9 without duplicates.
//
// Return true if the Sudoku board is valid, otherwise return false
//
// Note: A board does not need to be full or be solvable to be valid.
// ------------------------------------------------------------------------------------------------

#include <array>
#include "../common.hpp"

static constexpr char EMPTY_SLOT = '.';

using SudokuBoard  = std::array<std::array<char, 9>, 9>;
using DigitCounter = std::array<uint8_t, 9>;

static inline uint8_t as_number(char c) {
    return static_cast<uint8_t>(c - '0');
}

static inline bool valid_section(DigitCounter const& digits_found) {
    bool valid = true;
    for (uint8_t count : digits_found) {
        valid &= (count < 2);
    }
    return valid;
}

static bool is_valid_sudoku_board(SudokuBoard const& board) {
    bool valid_rows = true;
    {
        for (uint32_t row_idx = 0; (row_idx < 9) && valid_rows; ++row_idx) {
            DigitCounter digits_found = {};

            auto const& row = board[row_idx];
            for (char slot : row) {
                if (slot != EMPTY_SLOT) ++digits_found[as_number(slot) - 1];
            }

            valid_rows &= valid_section(digits_found);
        }
    }

    if (!valid_rows) return false;

    bool valid_columns = true;
    {
        // @NOTE: This does obviously non-optimal memory access.
        for (uint32_t column = 0; (column < 9) && valid_columns; ++column) {
            DigitCounter digits_found = {};

            for (auto const& row : board) {
                char slot = row[column];
                if (slot != EMPTY_SLOT) ++digits_found[as_number(slot) - 1];
            }

            valid_columns &= valid_section(digits_found);
        }
    }

    if (!valid_columns) return false;

    bool valid_blocks = true;
    {
        for (uint32_t row_of_blocks = 0; (row_of_blocks < 3) && valid_blocks; ++row_of_blocks) {
            DigitCounter digits_found_block0 = {};
            DigitCounter digits_found_block1 = {};
            DigitCounter digits_found_block2 = {};

            uint32_t end_row_idx = (row_of_blocks + 1) * 3;
            for (uint32_t row_idx = row_of_blocks * 3; row_idx < end_row_idx; ++row_idx) {
                auto const& row = board[row_idx];

                for (uint32_t block0_column = 0; block0_column < 3; ++block0_column) {
                    char slot = row[block0_column];
                    if (slot != EMPTY_SLOT) ++digits_found_block0[as_number(slot) - 1];
                }

                for (uint32_t block1_column = 3; block1_column < 6; ++block1_column) {
                    char slot = row[block1_column];
                    if (slot != EMPTY_SLOT) ++digits_found_block1[as_number(slot) - 1];
                }

                for (uint32_t block2_column = 6; block2_column < 9; ++block2_column) {
                    char slot = row[block2_column];
                    if (slot != EMPTY_SLOT) ++digits_found_block2[as_number(slot) - 1];
                }
            }

            valid_blocks &= (valid_section(digits_found_block0) && valid_section(digits_found_block1) && valid_section(digits_found_block2));
        }
    }

    return (valid_rows && valid_columns && valid_blocks);
}

int main() {
    // Test 1.
    {
        SudokuBoard board = {
            {{'1', '2', '.', '.', '3', '.', '.', '.', '.'},
             {'4', '.', '.', '5', '.', '.', '.', '.', '.'},
             {'.', '9', '8', '.', '.', '.', '.', '.', '3'},
             {'5', '.', '.', '.', '6', '.', '.', '.', '4'},
             {'.', '.', '.', '8', '.', '3', '.', '.', '5'},
             {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
             {'.', '.', '.', '.', '.', '.', '2', '.', '.'},
             {'.', '.', '.', '4', '1', '9', '.', '.', '8'},
             {'.', '.', '.', '.', '8', '.', '.', '7', '9'}}};

        assert(is_valid_sudoku_board(board));
    }

    // Test 2.
    {
        SudokuBoard board = {
            {{'1', '2', '.', '.', '3', '.', '.', '.', '.'},
             {'4', '.', '.', '5', '.', '.', '.', '.', '.'},
             {'.', '9', '8', '.', '.', '.', '.', '.', '3'},
             {'5', '.', '.', '.', '6', '.', '.', '.', '4'},
             {'.', '.', '.', '8', '.', '3', '.', '.', '5'},
             {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
             {'.', '.', '.', '.', '.', '.', '2', '.', '.'},
             {'.', '.', '.', '4', '1', '9', '.', '.', '8'},
             {'2', '3', '.', '.', '8', '.', '.', '7', '9'}}};

        assert(is_valid_sudoku_board(board));
    }

    // Test 3.
    {
        SudokuBoard board = {
            {{'1', '2', '.', '.', '3', '.', '.', '.', '.'},
             {'4', '.', '.', '5', '.', '.', '.', '.', '.'},
             {'.', '9', '1', '.', '.', '.', '.', '.', '3'},
             {'5', '.', '.', '.', '6', '.', '.', '.', '4'},
             {'.', '.', '.', '8', '.', '3', '.', '.', '5'},
             {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
             {'.', '.', '.', '.', '.', '.', '2', '.', '.'},
             {'.', '.', '.', '4', '1', '9', '.', '.', '8'},
             {'.', '.', '.', '.', '8', '.', '.', '7', '9'}}};

        assert(!is_valid_sudoku_board(board));
    }

    // Test 4.
    {
        SudokuBoard board = {
            {{'1', '2', '.', '.', '3', '.', '.', '.', '.'},
             {'4', '.', '.', '5', '.', '.', '.', '.', '.'},
             {'.', '9', '8', '.', '.', '.', '.', '.', '3'},
             {'5', '.', '.', '.', '6', '.', '.', '.', '4'},
             {'.', '.', '.', '8', '.', '3', '.', '.', '5'},
             {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
             {'.', '.', '.', '.', '.', '.', '2', '.', '7'},
             {'.', '.', '.', '4', '1', '9', '.', '.', '8'},
             {'.', '.', '.', '.', '8', '.', '.', '7', '9'}}};

        assert(!is_valid_sudoku_board(board));
    }

    report_success();
    return 0;
}
