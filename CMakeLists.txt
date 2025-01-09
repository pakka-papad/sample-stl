cmake_minimum_required(VERSION 3.10)

project(sample-stl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(test_vector tests/test_vector.cpp)
add_executable(test_unordered_map tests/test_unordered_map.cpp)
add_executable(test_forward_list tests/test_forward_list.cpp)
