# Intel_IXN_SightPP
MSc IXN Group Thesis in collaboraiton with Intel
## Introduction
## Overview
## Core sources

## Installation and Development
### Packages required
This project uses [Vcpkg](https://github.com/Microsoft/vcpkg) for library dependencies. Install the following libraries with Vcpkg to run the solution (make sure to install the 64-bit versions, as this system is being developed with 64-bit systems in mind):

* realsense2
* opencv3
* tbb
* websocketpp
* spdlog


### Installation and Run Manual of CMAKE
This section is a instruciton guide on how to install and run cmake. This will include a guide for building in windowns power shell as well as within Microsofts Visual Studio 2019. Runing the system with cmake has the same requirements as with the origional version. This guide includes the use of vcpkg and it is assumed that cmake is installed on your system.

#### Open with VS
- Start up visual studio an select "Open local Folder".
- Navigate inside /Intel_SighPP_CMake and click select folder.
- Rightclick on the top level CMakeLists.txt and select CMake Settings. This will open a new json page to instruct VS how to build the system.
- Following, Add the path to your vcpkg.cmake file to "CMake Toolchain file". This is roughly "<vcpkg path>/scritps/buildsystems/vcpkg.cmake".
- You will now be able to compile the system given you have installed everything correctly.
- Rightclick on the top level CMakeLists.txt and click Generate cache. This will build the base build instructions for the cmake system.
- Finally to build the executable under the build tab select rebuild all.
- The executable files will now be within the ./build directory.

#### Running the software within VS
- now that you have built the cmake, you can run the system in debug mode from VS.
- To this there are a few options.
- Option 1, target view:
  - in this method, first navigate to the solution explorer and to the right to the house icon.
  - Right click the "switch view" icon and select cmake target view.
  - You will now see the exeecutables of hte project.
  - To run the main project, navigate to "Intel_SightPP"
  - At this point if you would like to edit the run arguments right-click the executable and select "add debug configureation."
    - within the json object add the attribute args:[ \"\" ] and add the arguements you required
  - Finally: Either select Debug within the executable or set the item as Startup item and run from the top panel.
- Option 2,
  - To set the run arguments, right click on the top level cmake and seelect add build configureation, select default.
    - within the json object add the attribute args:[ \"\" ] and add the arguements you required
  -  Next you can either right click on the tope txt file and select debug or you can set the txt file as the STartup item and run the software from the top control panel as usual.

#### Building Cmake in terminal
- To build the cmake in terminal, navigate to the top file of the cmake repo.
- Create a new folder that you would like to build the repo in.
- Run the following command to build the repo into a visual studio source. Change the vcpkg location to your own path. Note: one could also remove the "Visual Studio 16 2019" to build it into your systems default Packages.
```bash
cmake .. -G "Visual Studio 16 2019" "-DCMAKE_TOOLCHAIN_FILE=<Path to vcpkg>/vcpkg/scripts/buildsystems/vcpkg.cmake"
```
-Note that it may not work the first time and a second run may be required as sometimes vcpkg may not be found at first.
- This should build the system similar to how it builds within the VS version.
- Finally to create the executable run the following command:
```bash
Windows:
camke --build .
```
```bash
OS/linux:
make .
```
- The executables will now be found within bin/debug
- Note: You will also now be able to open the VS project file via the *.sln* file within the build folder.
- run the
## User Manual
## Future work
## Contrubuting
## Acknowledment
