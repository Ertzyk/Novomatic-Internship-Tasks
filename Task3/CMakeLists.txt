cmake_minimum_required(VERSION 3.15)
project(NovomaticTask3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

add_executable(run_tests 
    src/LogParser.cpp
    src/QueryEngine.cpp
    tests/test.cpp
)

target_link_libraries(run_tests PRIVATE GTest::gtest_main)

add_executable(log_analyzer 
    src/main.cpp
    src/LogParser.cpp
    src/QueryEngine.cpp
)