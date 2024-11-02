cmake_minimum_required(VERSION 3.10)

project(sample-stl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(test_vector src/test_vector.cpp)
add_executable(test_unordered_map src/test_unordered_map.cpp)
