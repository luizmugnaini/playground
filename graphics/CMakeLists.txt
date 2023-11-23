cmake_minimum_required(VERSION 3.26)

project(
    graphics_playground
    VERSION 0.0.1
    LANGUAGES CXX C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)

set(
    GP_CXX_FLAGS
    "-std=c++17"
    "-Wall"
    "-Wextra"
    "-pedantic"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
include(FetchContent)

FetchContent_Declare(
    glfw 
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory("${glfw_SOURCE_DIR}" "${glfw_BINARY_DIR}")
endif()

add_library(
    glad
    "thirdparty/glad/src/gl.c"
    "thirdparty/glad/include/glad/gl.h"
)
target_include_directories(glad PUBLIC "thirdparty/glad/include")

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
      FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

add_executable(triforce "src/triforce.cpp")
target_compile_options(triforce PRIVATE ${GP_CXX_FLAGS})
set_target_properties(triforce PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "triforce")
target_link_libraries(triforce PRIVATE glfw glad)
