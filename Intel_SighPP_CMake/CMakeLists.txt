# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_TOOLCHAIN_FILE C:/Users/svens/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")
set(VCPKG_TARGET_TRIPLET x64-windows )

project (Intel_SighPP_CMake VERSION 0.1)

# Include sub-projects.
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

find_package(OpenCV CONFIG REQUIRED)
find_package(realsense2 CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)

add_subdirectory ("Intel_SighPP_CMake")

#find_package(GTest CONFIG REQUIRED PATHS C:/Users/svens/vcpkg/installed/x64-windows/share/gtest NO_DEFAULT_PATH)
#find_package(OpenCV CONFIG REQUIRED PATHS C:/Users/svens/vcpkg/installed/x64-windows/share/opencv3 NO_DEFAULT_PATH)
#find_package(realsense2 CONFIG REQUIRED PATHS C:/Users/svens/vcpkg/installed/x64-windows/share/realsense2 NO_DEFAULT_PATH)
#find_package(TBB CONFIG REQUIRED PATHS C:/Users/svens/vcpkg/installed/x64-windows/share/tbb NO_DEFAULT_PATH)