# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "/Users/badeand/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/191.7479.33/CLion.app/Contents/bin/cmake/mac/bin/cmake"

# The command to remove a file.
RM = "/Users/badeand/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/191.7479.33/CLion.app/Contents/bin/cmake/mac/bin/cmake" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/badeand/dev/labyrinth/experiments/balldetection_mag3110

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/badeand/dev/labyrinth/experiments/balldetection_mag3110/cmake-build-teensy31

# Include any dependencies generated for this target.
include CMakeFiles/teensy.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/teensy.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/teensy.dir/flags.make

CMakeFiles/teensy.dir/.pio/libdeps/teensy31/SparkFun_MAG3110_Magnetometer_Breakout_Arduino_Library_ID1600/src/SparkFun_MAG3110.cpp.o: CMakeFiles/teensy.dir/flags.make
CMakeFiles/teensy.dir/.pio/libdeps/teensy31/SparkFun_MAG3110_Magnetometer_Breakout_Arduino_Library_ID1600/src/SparkFun_MAG3110.cpp.o: ../.pio/libdeps/teensy31/SparkFun\ MAG3110\ Magnetometer\ Breakout\ Arduino\ Library_ID1600/src/SparkFun_MAG3110.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/badeand/dev/labyrinth/experiments/balldetection_mag3110/cmake-build-teensy31/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/teensy.dir/.pio/libdeps/teensy31/SparkFun_MAG3110_Magnetometer_Breakout_Arduino_Library_ID1600/src/SparkFun_MAG3110.cpp.o"
	/Users/badeand/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/teensy.dir/.pio/libdeps/teensy31/SparkFun_MAG3110_Magnetometer_Breakout_Arduino_Library_ID1600/src/SparkFun_MAG3110.cpp.o -c "/Users/badeand/dev/labyrinth/experiments/balldetection_mag3110/.pio/libdeps/teensy31/SparkFun MAG3110 Magnetometer Breakout Arduino Library_ID1600/src/SparkFun_MAG3110.cpp"

CMakeFiles/teensy.dir/.pio/libdeps/teensy31/SparkFun_MAG3110_Magnetometer_Breakout_Arduino_Library_ID1600/src/SparkFun_MAG3110.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/teensy.dir/.pio/libdeps/teensy31/SparkFun_MAG3110_Magnetometer_Breakout_Arduino_Library_ID1600/src/SparkFun_MAG3110.cpp.i"
	/Users/badeand/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/badeand/dev/labyrinth/experiments/balldetection_mag3110/.pio/libdeps/teensy31/SparkFun MAG3110 Magnetometer Breakout Arduino Library_ID1600/src/SparkFun_MAG3110.cpp" > CMakeFiles/teensy.dir/.pio/libdeps/teensy31/SparkFun_MAG3110_Magnetometer_Breakout_Arduino_Library_ID1600/src/SparkFun_MAG3110.cpp.i

CMakeFiles/teensy.dir/.pio/libdeps/teensy31/SparkFun_MAG3110_Magnetometer_Breakout_Arduino_Library_ID1600/src/SparkFun_MAG3110.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/teensy.dir/.pio/libdeps/teensy31/SparkFun_MAG3110_Magnetometer_Breakout_Arduino_Library_ID1600/src/SparkFun_MAG3110.cpp.s"
	/Users/badeand/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/badeand/dev/labyrinth/experiments/balldetection_mag3110/.pio/libdeps/teensy31/SparkFun MAG3110 Magnetometer Breakout Arduino Library_ID1600/src/SparkFun_MAG3110.cpp" -o CMakeFiles/teensy.dir/.pio/libdeps/teensy31/SparkFun_MAG3110_Magnetometer_Breakout_Arduino_Library_ID1600/src/SparkFun_MAG3110.cpp.s

CMakeFiles/teensy.dir/src/main.cpp.o: CMakeFiles/teensy.dir/flags.make
CMakeFiles/teensy.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/badeand/dev/labyrinth/experiments/balldetection_mag3110/cmake-build-teensy31/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/teensy.dir/src/main.cpp.o"
	/Users/badeand/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/teensy.dir/src/main.cpp.o -c /Users/badeand/dev/labyrinth/experiments/balldetection_mag3110/src/main.cpp

CMakeFiles/teensy.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/teensy.dir/src/main.cpp.i"
	/Users/badeand/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/badeand/dev/labyrinth/experiments/balldetection_mag3110/src/main.cpp > CMakeFiles/teensy.dir/src/main.cpp.i

CMakeFiles/teensy.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/teensy.dir/src/main.cpp.s"
	/Users/badeand/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/badeand/dev/labyrinth/experiments/balldetection_mag3110/src/main.cpp -o CMakeFiles/teensy.dir/src/main.cpp.s

# Object files for target teensy
teensy_OBJECTS = \
"CMakeFiles/teensy.dir/.pio/libdeps/teensy31/SparkFun_MAG3110_Magnetometer_Breakout_Arduino_Library_ID1600/src/SparkFun_MAG3110.cpp.o" \
"CMakeFiles/teensy.dir/src/main.cpp.o"

# External object files for target teensy
teensy_EXTERNAL_OBJECTS =

teensy: CMakeFiles/teensy.dir/.pio/libdeps/teensy31/SparkFun_MAG3110_Magnetometer_Breakout_Arduino_Library_ID1600/src/SparkFun_MAG3110.cpp.o
teensy: CMakeFiles/teensy.dir/src/main.cpp.o
teensy: CMakeFiles/teensy.dir/build.make
teensy: CMakeFiles/teensy.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/badeand/dev/labyrinth/experiments/balldetection_mag3110/cmake-build-teensy31/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable teensy"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/teensy.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/teensy.dir/build: teensy

.PHONY : CMakeFiles/teensy.dir/build

CMakeFiles/teensy.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/teensy.dir/cmake_clean.cmake
.PHONY : CMakeFiles/teensy.dir/clean

CMakeFiles/teensy.dir/depend:
	cd /Users/badeand/dev/labyrinth/experiments/balldetection_mag3110/cmake-build-teensy31 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/badeand/dev/labyrinth/experiments/balldetection_mag3110 /Users/badeand/dev/labyrinth/experiments/balldetection_mag3110 /Users/badeand/dev/labyrinth/experiments/balldetection_mag3110/cmake-build-teensy31 /Users/badeand/dev/labyrinth/experiments/balldetection_mag3110/cmake-build-teensy31 /Users/badeand/dev/labyrinth/experiments/balldetection_mag3110/cmake-build-teensy31/CMakeFiles/teensy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/teensy.dir/depend
