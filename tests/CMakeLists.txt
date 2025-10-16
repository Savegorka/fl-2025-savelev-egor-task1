enable_testing()

include_directories(../src)

add_executable(unit_test tests.cpp ../src/automatic.h ../src/automatic.cpp)

target_link_libraries(unit_test GTest::gtest GTest::gtest_main)

add_test(NAME MyTest COMMAND unit_test)
