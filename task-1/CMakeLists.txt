cmake_minimum_required(VERSION 3.10)
project(task-1 CXX)

include_directories(src)

add_executable(task-1
        src/main.cpp
        src/BitArray.cpp
)

add_executable(simple_bitarray_test
        simple_bitarray_test/simple_test.cpp
        src/BitArray.cpp
)


target_include_directories(simple_bitarray_test PRIVATE src)

# Поиск GTest
find_package(GTest QUIET)
if(GTest_FOUND)
    target_link_libraries(simple_bitarray_test GTest::gtest GTest::gtest_main)
    message(STATUS "GTest found, linking with it")
else()
    message(STATUS "GTest not found, building without it")
endif()

include(CTest)
enable_testing()
add_test(NAME simple_bitarray_tests COMMAND simple_bitarray_test)
