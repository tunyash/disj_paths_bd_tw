cmake_minimum_required(VERSION 3.17)
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

target_link_libraries(disj_paths_bd_tw PUBLIC path_dec)

add_executable(test src/test.cpp)
add_test(NAME test0 COMMAND test)
target_link_libraries(test PUBLIC path_dec)

add_library(nice_path_dec src/nice-path-decomposition/nice-path-decomposition.cpp)
add_executable(nice_test src/nice-path-decomposition/test.cpp)
target_link_libraries(nice_test PUBLIC nice_path_dec path_dec)