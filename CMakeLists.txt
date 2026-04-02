cmake_minimum_required(VERSION 3.14)
project(SortCompare)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(SortCompare 
    src/main.cpp
    src/sort_algorithms.cpp
    src/data_generator.cpp
    src/timer.cpp
    src/utils.cpp
)

# 输出路径
set_target_properties(SortCompare PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
)