# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project (Intel_SighPP_CMake VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-DFMT_HEADER_ONLY)

find_package(OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
find_package(realsense2 REQUIRED PATHS /Users/xingdacen/Downloads/vcpkg/installed/x64-osx/share/realsense2 NO_DEFAULT_PATH)
find_package(TBB REQUIRED PATHS /Users/xingdacen/Downloads/vcpkg/installed/x64-osx/share/tbb NO_DEFAULT_PATH)
find_package(websocketpp REQUIRED PATHS /Users/xingdacen/Downloads/vcpkg/installed/x64-osx/share/websocketpp NO_DEFAULT_PATH)
find_package(GTest REQUIRED PATHS /Users/xingdacen/Downloads/vcpkg/installed/x64-osx/share/gtest NO_DEFAULT_PATH)
find_package(spdlog REQUIRED PATHS /Users/xingdacen/Downloads/vcpkg/installed/x64-osx/share/spdlog NO_DEFAULT_PATH)

add_subdirectory ("Intel_SighPP_CMake")

