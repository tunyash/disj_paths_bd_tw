cmake_minimum_required(VERSION 3.19)
project(disj_paths_bd_tw)

set(CMAKE_CXX_STANDARD 14)

include_directories("/home/tunyash/boost_1_76_0/")
add_executable(disj_paths_bd_tw main.cpp)
add_library(path_dec src/path-decomposition.cpp)

target_link_libraries(disj_paths_bd_tw PUBLIC path_dec)

add_executable(test src/test.cpp)
add_test(NAME test0 COMMAND test)
target_link_libraries(test PUBLIC path_dec)