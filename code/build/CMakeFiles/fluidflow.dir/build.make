# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.7.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.7.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/garychen/Documents/CS184/FluidFlow

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/garychen/Documents/CS184/FluidFlow/build

# Include any dependencies generated for this target.
include CMakeFiles/fluidflow.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/fluidflow.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fluidflow.dir/flags.make

CMakeFiles/fluidflow.dir/src/GLShader.cpp.o: CMakeFiles/fluidflow.dir/flags.make
CMakeFiles/fluidflow.dir/src/GLShader.cpp.o: ../src/GLShader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/garychen/Documents/CS184/FluidFlow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/fluidflow.dir/src/GLShader.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fluidflow.dir/src/GLShader.cpp.o -c /Users/garychen/Documents/CS184/FluidFlow/src/GLShader.cpp

CMakeFiles/fluidflow.dir/src/GLShader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fluidflow.dir/src/GLShader.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/garychen/Documents/CS184/FluidFlow/src/GLShader.cpp > CMakeFiles/fluidflow.dir/src/GLShader.cpp.i

CMakeFiles/fluidflow.dir/src/GLShader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fluidflow.dir/src/GLShader.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/garychen/Documents/CS184/FluidFlow/src/GLShader.cpp -o CMakeFiles/fluidflow.dir/src/GLShader.cpp.s

CMakeFiles/fluidflow.dir/src/GLShader.cpp.o.requires:

.PHONY : CMakeFiles/fluidflow.dir/src/GLShader.cpp.o.requires

CMakeFiles/fluidflow.dir/src/GLShader.cpp.o.provides: CMakeFiles/fluidflow.dir/src/GLShader.cpp.o.requires
	$(MAKE) -f CMakeFiles/fluidflow.dir/build.make CMakeFiles/fluidflow.dir/src/GLShader.cpp.o.provides.build
.PHONY : CMakeFiles/fluidflow.dir/src/GLShader.cpp.o.provides

CMakeFiles/fluidflow.dir/src/GLShader.cpp.o.provides.build: CMakeFiles/fluidflow.dir/src/GLShader.cpp.o


CMakeFiles/fluidflow.dir/src/camera.cpp.o: CMakeFiles/fluidflow.dir/flags.make
CMakeFiles/fluidflow.dir/src/camera.cpp.o: ../src/camera.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/garychen/Documents/CS184/FluidFlow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/fluidflow.dir/src/camera.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fluidflow.dir/src/camera.cpp.o -c /Users/garychen/Documents/CS184/FluidFlow/src/camera.cpp

CMakeFiles/fluidflow.dir/src/camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fluidflow.dir/src/camera.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/garychen/Documents/CS184/FluidFlow/src/camera.cpp > CMakeFiles/fluidflow.dir/src/camera.cpp.i

CMakeFiles/fluidflow.dir/src/camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fluidflow.dir/src/camera.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/garychen/Documents/CS184/FluidFlow/src/camera.cpp -o CMakeFiles/fluidflow.dir/src/camera.cpp.s

CMakeFiles/fluidflow.dir/src/camera.cpp.o.requires:

.PHONY : CMakeFiles/fluidflow.dir/src/camera.cpp.o.requires

CMakeFiles/fluidflow.dir/src/camera.cpp.o.provides: CMakeFiles/fluidflow.dir/src/camera.cpp.o.requires
	$(MAKE) -f CMakeFiles/fluidflow.dir/build.make CMakeFiles/fluidflow.dir/src/camera.cpp.o.provides.build
.PHONY : CMakeFiles/fluidflow.dir/src/camera.cpp.o.provides

CMakeFiles/fluidflow.dir/src/camera.cpp.o.provides.build: CMakeFiles/fluidflow.dir/src/camera.cpp.o


CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.o: CMakeFiles/fluidflow.dir/flags.make
CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.o: ../src/fluid_simulation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/garychen/Documents/CS184/FluidFlow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.o -c /Users/garychen/Documents/CS184/FluidFlow/src/fluid_simulation.cpp

CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/garychen/Documents/CS184/FluidFlow/src/fluid_simulation.cpp > CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.i

CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/garychen/Documents/CS184/FluidFlow/src/fluid_simulation.cpp -o CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.s

CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.o.requires:

.PHONY : CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.o.requires

CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.o.provides: CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.o.requires
	$(MAKE) -f CMakeFiles/fluidflow.dir/build.make CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.o.provides.build
.PHONY : CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.o.provides

CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.o.provides.build: CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.o


CMakeFiles/fluidflow.dir/src/lodepng.cpp.o: CMakeFiles/fluidflow.dir/flags.make
CMakeFiles/fluidflow.dir/src/lodepng.cpp.o: ../src/lodepng.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/garychen/Documents/CS184/FluidFlow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/fluidflow.dir/src/lodepng.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fluidflow.dir/src/lodepng.cpp.o -c /Users/garychen/Documents/CS184/FluidFlow/src/lodepng.cpp

CMakeFiles/fluidflow.dir/src/lodepng.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fluidflow.dir/src/lodepng.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/garychen/Documents/CS184/FluidFlow/src/lodepng.cpp > CMakeFiles/fluidflow.dir/src/lodepng.cpp.i

CMakeFiles/fluidflow.dir/src/lodepng.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fluidflow.dir/src/lodepng.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/garychen/Documents/CS184/FluidFlow/src/lodepng.cpp -o CMakeFiles/fluidflow.dir/src/lodepng.cpp.s

CMakeFiles/fluidflow.dir/src/lodepng.cpp.o.requires:

.PHONY : CMakeFiles/fluidflow.dir/src/lodepng.cpp.o.requires

CMakeFiles/fluidflow.dir/src/lodepng.cpp.o.provides: CMakeFiles/fluidflow.dir/src/lodepng.cpp.o.requires
	$(MAKE) -f CMakeFiles/fluidflow.dir/build.make CMakeFiles/fluidflow.dir/src/lodepng.cpp.o.provides.build
.PHONY : CMakeFiles/fluidflow.dir/src/lodepng.cpp.o.provides

CMakeFiles/fluidflow.dir/src/lodepng.cpp.o.provides.build: CMakeFiles/fluidflow.dir/src/lodepng.cpp.o


CMakeFiles/fluidflow.dir/src/main.cpp.o: CMakeFiles/fluidflow.dir/flags.make
CMakeFiles/fluidflow.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/garychen/Documents/CS184/FluidFlow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/fluidflow.dir/src/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fluidflow.dir/src/main.cpp.o -c /Users/garychen/Documents/CS184/FluidFlow/src/main.cpp

CMakeFiles/fluidflow.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fluidflow.dir/src/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/garychen/Documents/CS184/FluidFlow/src/main.cpp > CMakeFiles/fluidflow.dir/src/main.cpp.i

CMakeFiles/fluidflow.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fluidflow.dir/src/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/garychen/Documents/CS184/FluidFlow/src/main.cpp -o CMakeFiles/fluidflow.dir/src/main.cpp.s

CMakeFiles/fluidflow.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/fluidflow.dir/src/main.cpp.o.requires

CMakeFiles/fluidflow.dir/src/main.cpp.o.provides: CMakeFiles/fluidflow.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/fluidflow.dir/build.make CMakeFiles/fluidflow.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/fluidflow.dir/src/main.cpp.o.provides

CMakeFiles/fluidflow.dir/src/main.cpp.o.provides.build: CMakeFiles/fluidflow.dir/src/main.cpp.o


CMakeFiles/fluidflow.dir/src/glad.c.o: CMakeFiles/fluidflow.dir/flags.make
CMakeFiles/fluidflow.dir/src/glad.c.o: ../src/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/garychen/Documents/CS184/FluidFlow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/fluidflow.dir/src/glad.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/fluidflow.dir/src/glad.c.o   -c /Users/garychen/Documents/CS184/FluidFlow/src/glad.c

CMakeFiles/fluidflow.dir/src/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/fluidflow.dir/src/glad.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/garychen/Documents/CS184/FluidFlow/src/glad.c > CMakeFiles/fluidflow.dir/src/glad.c.i

CMakeFiles/fluidflow.dir/src/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/fluidflow.dir/src/glad.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/garychen/Documents/CS184/FluidFlow/src/glad.c -o CMakeFiles/fluidflow.dir/src/glad.c.s

CMakeFiles/fluidflow.dir/src/glad.c.o.requires:

.PHONY : CMakeFiles/fluidflow.dir/src/glad.c.o.requires

CMakeFiles/fluidflow.dir/src/glad.c.o.provides: CMakeFiles/fluidflow.dir/src/glad.c.o.requires
	$(MAKE) -f CMakeFiles/fluidflow.dir/build.make CMakeFiles/fluidflow.dir/src/glad.c.o.provides.build
.PHONY : CMakeFiles/fluidflow.dir/src/glad.c.o.provides

CMakeFiles/fluidflow.dir/src/glad.c.o.provides.build: CMakeFiles/fluidflow.dir/src/glad.c.o


# Object files for target fluidflow
fluidflow_OBJECTS = \
"CMakeFiles/fluidflow.dir/src/GLShader.cpp.o" \
"CMakeFiles/fluidflow.dir/src/camera.cpp.o" \
"CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.o" \
"CMakeFiles/fluidflow.dir/src/lodepng.cpp.o" \
"CMakeFiles/fluidflow.dir/src/main.cpp.o" \
"CMakeFiles/fluidflow.dir/src/glad.c.o"

# External object files for target fluidflow
fluidflow_EXTERNAL_OBJECTS =

fluidflow: CMakeFiles/fluidflow.dir/src/GLShader.cpp.o
fluidflow: CMakeFiles/fluidflow.dir/src/camera.cpp.o
fluidflow: CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.o
fluidflow: CMakeFiles/fluidflow.dir/src/lodepng.cpp.o
fluidflow: CMakeFiles/fluidflow.dir/src/main.cpp.o
fluidflow: CMakeFiles/fluidflow.dir/src/glad.c.o
fluidflow: CMakeFiles/fluidflow.dir/build.make
fluidflow: glfw/src/libglfw3.a
fluidflow: /usr/local/lib/libopencv_videostab.2.4.13.dylib
fluidflow: /usr/local/lib/libopencv_ts.a
fluidflow: /usr/local/lib/libopencv_superres.2.4.13.dylib
fluidflow: /usr/local/lib/libopencv_stitching.2.4.13.dylib
fluidflow: /usr/local/lib/libopencv_contrib.2.4.13.dylib
fluidflow: /usr/local/lib/libopencv_nonfree.2.4.13.dylib
fluidflow: /usr/local/lib/libopencv_ocl.2.4.13.dylib
fluidflow: /usr/local/lib/libopencv_gpu.2.4.13.dylib
fluidflow: /usr/local/lib/libopencv_photo.2.4.13.dylib
fluidflow: /usr/local/lib/libopencv_objdetect.2.4.13.dylib
fluidflow: /usr/local/lib/libopencv_legacy.2.4.13.dylib
fluidflow: /usr/local/lib/libopencv_video.2.4.13.dylib
fluidflow: /usr/local/lib/libopencv_ml.2.4.13.dylib
fluidflow: /usr/local/lib/libopencv_calib3d.2.4.13.dylib
fluidflow: /usr/local/lib/libopencv_features2d.2.4.13.dylib
fluidflow: /usr/local/lib/libopencv_highgui.2.4.13.dylib
fluidflow: /usr/local/lib/libopencv_imgproc.2.4.13.dylib
fluidflow: /usr/local/lib/libopencv_flann.2.4.13.dylib
fluidflow: /usr/local/lib/libopencv_core.2.4.13.dylib
fluidflow: CMakeFiles/fluidflow.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/garychen/Documents/CS184/FluidFlow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable fluidflow"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fluidflow.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fluidflow.dir/build: fluidflow

.PHONY : CMakeFiles/fluidflow.dir/build

CMakeFiles/fluidflow.dir/requires: CMakeFiles/fluidflow.dir/src/GLShader.cpp.o.requires
CMakeFiles/fluidflow.dir/requires: CMakeFiles/fluidflow.dir/src/camera.cpp.o.requires
CMakeFiles/fluidflow.dir/requires: CMakeFiles/fluidflow.dir/src/fluid_simulation.cpp.o.requires
CMakeFiles/fluidflow.dir/requires: CMakeFiles/fluidflow.dir/src/lodepng.cpp.o.requires
CMakeFiles/fluidflow.dir/requires: CMakeFiles/fluidflow.dir/src/main.cpp.o.requires
CMakeFiles/fluidflow.dir/requires: CMakeFiles/fluidflow.dir/src/glad.c.o.requires

.PHONY : CMakeFiles/fluidflow.dir/requires

CMakeFiles/fluidflow.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fluidflow.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fluidflow.dir/clean

CMakeFiles/fluidflow.dir/depend:
	cd /Users/garychen/Documents/CS184/FluidFlow/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/garychen/Documents/CS184/FluidFlow /Users/garychen/Documents/CS184/FluidFlow /Users/garychen/Documents/CS184/FluidFlow/build /Users/garychen/Documents/CS184/FluidFlow/build /Users/garychen/Documents/CS184/FluidFlow/build/CMakeFiles/fluidflow.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fluidflow.dir/depend

