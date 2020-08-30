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
* opencv3[contrib]
* tbb
* websocketpp
* spdlog


### Installation and Run Manual of CMAKE
This section is an instruction guide on how to install and run CMake. This will include a guide for building in windows power shell as well as within Microsofts Visual Studio 2019. Running the system with CMake has the same requirements as with the original version. This guide includes the use of vcpkg, and it is assumed that CMake is installed on your system.

#### Open with VS
- Startup visual studio and select "Open local Folder".
- Navigate inside /Intel_SighPP_CMake and click select folder.
- Rightclick on the top-level CMakeLists.txt and select CMake Settings. This will open a new JSON page to instruct VS on how to build the system.
- Following, Add the path to your vcpkg.cmake file to "CMake Toolchain file". This is roughly "<vcpkg path>/scritps/buildsystems/vcpkg.cmake".
- You will now be able to compile the system given you have installed everything correctly.
- Rightclick on the top-level CMakeLists.txt and click Generate cache. This will build the base build instructions for the CMake system.
- Finally, to build the executable under the build tab select rebuild all.
- The executable files will now be within the ./build directory.

#### Running the software within VS
- Now that you have built the CMake, you can run the system in debug mode from VS.
- Note: You may need to copy the models folder to the location of the executable as this is needed, especially if you changed the build directory.
- There are a few options.
- Option 1, target view:
  - In this method, first, navigate to the solution explorer to the right to the home icon.
  - Right-click the "switch view" icon and select the CMake target view.
  - You will now see the executables of the project.
  - To run the main project, navigate to "Intel_SightPP"
  - At this point, if you would like to edit the run arguments right-click the executable and select "add debug configuration."
    - Within the JSON object add the attribute args:[ \"\" ] and add the arguments you required
  - Finally: Either select Debug within the executable or set the item as Startup item and run from the top panel.
- Option 2,
  - To set the run arguments, right-click on the top-level CMake and select add build configuration, select default.
    - within the JSON object add the attribute args:[ \"\" ] and add the arguments you required
  -  Next, either, you can right-click on the top .txt file and select debug, or you can set the text file as the Startup Item and run the software from the top control panel as usual.

#### Building Cmake in terminal
- To build the CMake in the terminal, navigate to the top file of the CMake repo.
- Create a new folder that you would like to build the repo in
  - Please use a name from this list so it is ignored from the repo: output,build,out,compile,debug or test
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
- The executables will now be found within your desired build folder. bin/debug by default.
- Note: You will also be able to open the VS project file via the *.sln* file within the build folder.
- Note: You may need to copy the models folder to the location of the executable as this is needed, especially if you changed the build directory.
- Following you can run the executable such as Inter_SightPP.exe with the flags following.

###flags
Flags for running the system include:
- "realsense" = run the system using a realsense camera
- "-color" = view the rgb output from the service for testing
- "-depth" = view the deoth output from the service for testing
- "-rec \<filename\>" = record input to a file
- "-play \<filename\>" = set imput to a file
- "-port \<port number\>" = sets the websocket port, default is 7979
## User Manual
## Future work
## Contrubuting
## Acknowledgement
