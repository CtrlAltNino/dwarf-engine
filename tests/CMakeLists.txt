include(GoogleTest)

# cmake_minimum_required(VERSION 3.22)
file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(${testTarget} TestEntry.cpp)

smtg_add_subdirectories()

find_package(GTest CONFIG REQUIRED)
target_link_libraries(${testTarget} PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main ${libname})

gtest_add_tests(TARGET ${testTarget})