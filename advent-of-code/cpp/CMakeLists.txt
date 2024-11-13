cmake_minimum_required(VERSION 3.26)

project(
    aoc
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)

set(
    AOC_CXX_FLAGS
    "-std=c++17"
    "-Wall"
    "-Wextra"
    "-pedantic"
    "-Wshadow" # warn the user if a variable declaration shadows a parent variable.
    "-Wnon-virtual-dtor" # warn the user if a class with virtual functions has a
                         # non-virtual destructor. This helps catch hard to
                         # track down memory errors.
    "-Wold-style-cast" # warn for c-style casts
    "-Wcast-align"     # warn for potential performance problem casts
    "-Wunused"         # warn on anything being unused
    "-Woverloaded-virtual" # warn if you overload (not override) a virtual
                           # function
    "-Wpedantic"   # warn if non-standard C++ is used
    "-Wconversion" # warn on type conversions that may lose data
    "-Wsign-conversion"  # warn on sign conversions
    "-Wnull-dereference" # warn if a null dereference is detected
    "-Wdouble-promotion" # warn if float is implicit promoted to double
    "-Wformat=2" # warn on security issues around functions that format output
                 # (ie printf)
    "-Wmisleading-indentation" # warn if indentation implies blocks where blocks
                               # do not exist
)
set(
    AOC_SAN_CXX_FLAGS
    "-fsanitize=address" # Enable AddressSanitizer, a fast memory error detector.
                         # Memory access instructions are instrumented to detect
                         # out-of-bounds and use-after-free bugs. The option
                         # enables -fsanitize-address-use-after-scope.
    "-fsanitize=pointer-compare" # Instrument comparison operation (<, <=, >, >=) with
                                 # pointer operands. The option must be combined with
                                 # either -fsanitize=kernel-address or -fsanitize=address
    "-fsanitize=pointer-subtract" # Instrument subtraction with pointer operands.
                                  # The option must be combined with either
                                  # -fsanitize=kernel-address or -fsanitize=address.
    "-fsanitize=shadow-call-stack" # Enable ShadowCallStack, a security enhancement
                                   # mechanism used to protect programs against return
                                   # address overwrites (e.g. stack buffer overflows.)
    "-fsanitize=leak" # Enable LeakSanitizer, a memory leak detector. This option only
                      # matters for linking of executables.
    "-fsanitize=undefined" # Enable UndefinedBehaviorSanitizer.
    "-fstack-protector-strong" # Like -fstack-protector but includes additional functions
                               # to be protected — those that have local array definitions,
                               # or have references to local frame addresses.
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
      FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

add_executable(aoc23day1 "src/aoc23/day1.cpp")
target_compile_options(aoc23day1 PRIVATE ${AOC_CXX_FLAGS} ${AOC_SAN_CXX_FLAGS})
set_target_properties(aoc23day1 PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "aoc23day1")
target_link_libraries(aoc23day1 PRIVATE ${AOC_SAN_CXX_FLAGS})
