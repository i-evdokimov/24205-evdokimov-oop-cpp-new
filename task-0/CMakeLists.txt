cmake_minimum_required(VERSION 3.20)
project(lab0)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME}
        src/FileHandler.cpp
        src/TextProcessor.cpp
        src/WordCounter.cpp
        src/CSVExporter.cpp
        src/main.cpp
)

add_executable(simple_tests
        tests_lab0/simple_test.cpp
        src/FileHandler.cpp
        src/TextProcessor.cpp
        src/WordCounter.cpp
        src/CSVExporter.cpp
)