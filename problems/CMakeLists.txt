# ------------------------------------------------------------------------------
# Description: CMake-based build file for the solutions to interview-like problems.
# Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
# ------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.22)

project(
    problems LANGUAGES CXX C
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# ------------------------------------------------------------------------------
# Sources
# ------------------------------------------------------------------------------

list(
    APPEND EASY_PROBLEMS
    "duplicate_integer"
    "is_anagram"
    "unique_elements"
    "two_sum"
    "two_sum_2"
    "valid_palindrome"
    "valid_parentheses"
)

list(
    APPEND MEDIUM_PROBLEMS
    "anagram_groups"
    "encode_decode_strings"
    "product_except_self"
    "top_k_frequent"
    "valid_sudoku"
    "longest_consecutive"
    "three_sum"
    "container_with_most_water"
    "longest_substring_with_unique_characters"
    "longest_repeating_character_replacement"
    "minimum_stack"
    "evaluate_reverse_polish_notation"
    "search_in_2d_matrix"
    "string_to_integer"
)

list(
    APPEND HARD_PROBLEMS
    "trapping_rain_water"
)

# ------------------------------------------------------------------------------
# Compiler configs.
# ------------------------------------------------------------------------------

set(
    MSVC_FLAGS
    "/std:c++20"
    "/nologo"
    "/EHc" # Unfortunately we have to allow exceptions because we'll be using the STL.
    "/WX"
    "/W4"
    "/Z7"
    "/Ob1"
)

# ------------------------------------------------------------------------------
# Executables.
# ------------------------------------------------------------------------------

function(build_solutions PROBLEM_CATEGORY PROBLEM_LIST)
    foreach(p IN LISTS ${PROBLEM_LIST})
        add_executable(${p} "${CMAKE_SOURCE_DIR}/${PROBLEM_CATEGORY}/${p}.cpp")
        target_include_directories(${p} PRIVATE "${CMAKE_SOURCE_DIR}/include")
        target_compile_options(${p} PRIVATE ${MSVC_FLAGS})
    endforeach()
endfunction()

build_solutions("easy"   EASY_PROBLEMS)
build_solutions("medium" MEDIUM_PROBLEMS)
build_solutions("hard"   HARD_PROBLEMS)
