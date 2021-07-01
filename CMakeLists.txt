cmake_minimum_required(VERSION 3.17)
project(disj_paths_bd_tw)

set(CMAKE_CXX_STANDARD 14)

if (WIN32)
    include_directories("C:/boost_1_76_0")
endif(WIN32)
if (UNIX)
    include_directories("~/boost_1_76_0/")
endif(UNIX)


add_library(path_dec src/path-decomposition.cpp)
add_library(centroid_tree src/centroid-tree.cpp)
add_library(nice_path_dec src/nice-path-decomposition.cpp)
add_library(composed_graph src/composed-graph.cpp)

add_executable(test src/test.cpp src/nice-test.cpp src/composed-test.cpp)

target_link_libraries(test PUBLIC centroid_tree)
target_link_libraries(centroid_tree PUBLIC path_dec)
target_link_libraries(composed_graph PUBLIC nice_path_dec)
target_link_libraries(test PUBLIC path_dec)
target_link_libraries(test PUBLIC nice_path_dec)
target_link_libraries(test PUBLIC composed_graph)
target_link_libraries(test PUBLIC centroid_tree)

add_test(NAME test0 COMMAND test)
add_test(NAME nice_test0 COMMAND nice_test)
