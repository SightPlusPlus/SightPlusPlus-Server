# Intel_IXN_SightPP
MSc IXN Group Thesis in collaboraiton with Intel

## Packages required
This project uses [Vcpkg](https://github.com/Microsoft/vcpkg) for library dependencies. Install the following libraries with Vcpkg to run the solution (make sure to install the 64-bit versions, as this system is being developed with 64-bit systems in mind):

* realsense2
* opencv3
* tbb
* websocketpp
* spdlog


## Installation and Run Manual of CMAKE
This section is a instruciton guide on how to install and run cmake. This will include a guide for building in windowns power shell as well as within Microsofts Visual Studio 2019. Runing the system with cmake has the same requirements as with the origional version. This guide includes the use of vcpkg and it is assumed that cmake is installed on your system.

### Open with VS
- Start up visual studio an select "Open local Folder".
- Navigate inside /Intel_SighPP_CMake and click select folder.
- Rightclick on the top level CMakeLists.txt and select CMake Settings. This will open a new json page to instruct VS how to build the system.
- Following, Add the path to your vcpkg.cmake file to "CMake Toolchain file". This is roughly "<vcpkg path>/scritps/buildsystems/vcpkg.cmake".
-
- Rightclick on the top level CMakeLists.txt and click Generate cache. This will build the base build instructions for the cmake system.
-
### Building cmake in VS

### Building Cmake in terminal

### Runing System
