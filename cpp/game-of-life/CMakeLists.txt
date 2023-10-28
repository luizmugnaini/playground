cmake_minimum_required(VERSION 3.26)

project(
    game_of_life
    VERSION 0.0.1
    LANGUAGES CXX
    )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)

set(
    GOL_CXX_FLAGS
    "-std=c++17"
    "-Wall"
    "-Wextra"
    "-pedantic"
    )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

find_package(SDL2)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
      FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

add_executable(
    gol
    "src/main.cpp"
    )
target_compile_options(gol PRIVATE ${GOL_CXX_FLAGS})
set_target_properties(gol PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "gol")
target_link_libraries(gol PRIVATE SDL2)
