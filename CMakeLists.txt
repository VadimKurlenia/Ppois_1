cmake_minimum_required(VERSION 3.10)
project(Lab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(labcore STATIC src/Polynomial.cpp src/Set.cpp)
target_include_directories(labcore PUBLIC include)

add_executable(menu app/menu.cpp)
target_link_libraries(menu labcore)

add_executable(tests
    tests/main_tests.cpp
    tests/tests_polynomial.cpp
    tests/tests_set.cpp)
target_link_libraries(tests labcore UnitTest++)