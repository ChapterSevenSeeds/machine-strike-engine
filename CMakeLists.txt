cmake_minimum_required(VERSION 3.29.3)
project(machine-strike-engine VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_subdirectory(src)
add_subdirectory(tests)