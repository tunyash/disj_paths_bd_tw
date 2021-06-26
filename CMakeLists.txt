cmake_minimum_required(VERSION 3.19)
project(disj_paths_bd_tw)

set(CMAKE_CXX_STANDARD 14)

if (WIN32)
    include_directories("C:/boost_1_76_0")
endif(WIN32)
if (UNIX)
    include_directories("~/boost_1_76_0/")
endif(UNIX)

add_executable(disj_paths_bd_tw src/test.cpp)
add_library(path_dec src/path-decomposition.cpp)
add_library(centroid_tree src/centroid-tree.cpp)

add_executable(test src/test.cpp)
add_test(NAME test0 COMMAND test)
target_link_libraries(test PUBLIC path_dec)
target_link_libraries(test PUBLIC centroid_tree)
target_link_libraries(centroid_tree PUBLIC path_dec)