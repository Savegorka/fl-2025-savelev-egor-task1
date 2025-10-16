cmake_minimum_required(VERSION 3.22)
project(formal_languages_prac)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
find_package(GTest REQUIRED)
add_executable(main src/main.cpp src/automatic.h src/automatic.cpp)
enable_testing()
add_subdirectory(tests)
