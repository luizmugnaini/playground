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
    "two_sum"
)

list(
    APPEND MEDIUM_PROBLEMS
    "anagram_groups"
    "encode_decode_strings"
    "product_except_self"
    "top_k_frequent"
    "valid_sudoku"
)

list(
    APPEND HARD_PROBLEMS
    # Empty for now...
)

# ------------------------------------------------------------------------------
# Compiler configs.
# ------------------------------------------------------------------------------

set(
    MSVC_FLAGS
    "/std:c++20"
    "/nologo"
    "/EHsc" # Unfortunately we have to allow exceptions because we'll be using the STL.
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
        target_compile_options(${p} PRIVATE ${MSVC_FLAGS})
    endforeach()
endfunction()

build_solutions("easy"   EASY_PROBLEMS)
build_solutions("medium" MEDIUM_PROBLEMS)
build_solutions("hard"   HARD_PROBLEMS)
