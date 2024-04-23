cmake_minimum_required(VERSION 3.10.2)
add_executable(${testTarget} ApplicationTests.cpp)

enable_testing()
find_package(GTest CONFIG REQUIRED)
target_link_libraries(${testTarget} PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
add_test(AllTestsInMain ${testTarget})