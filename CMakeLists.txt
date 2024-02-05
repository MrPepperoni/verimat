cmake_minimum_required(VERSION 3.25)
project(verimat LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        f8d7d77c06936315286eb55f8de22cd23c188571 # release-1.14.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(verimatlang)
add_subdirectory(main)

