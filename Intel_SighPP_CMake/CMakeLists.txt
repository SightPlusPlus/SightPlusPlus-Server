# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project (Intel_SighPP_CMake VERSION 0.1)

find_package(OpenCV 3.4 CONFIG REQUIRED)
find_package(realsense2 CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)
find_package(websocketpp CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(spdlog 1.6 CONFIG REQUIRED)

add_subdirectory ("Intel_SighPP_CMake")
