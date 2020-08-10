# CMakeList.txt : CMake project for Intel_SighPP_CMake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.17)

enable_testing()
include(GoogleTest)
#find_package(GTest CONFIG REQUIRED)
#include(GoogleTest)
add_executable(test_demo "test.cpp")

target_link_libraries(test_demo gtest
)
#include_directories(${GTEST_INCLUDE_DIRS})
#target_link_libraries(tester PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
add_test(NAME test_demo COMMAND test_demo)
#add_subdirectory("models")

