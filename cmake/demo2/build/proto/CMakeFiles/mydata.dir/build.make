# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ddming/Study/cmake/demo2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ddming/Study/cmake/demo2/build

# Include any dependencies generated for this target.
include proto/CMakeFiles/mydata.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include proto/CMakeFiles/mydata.dir/compiler_depend.make

# Include the progress variables for this target.
include proto/CMakeFiles/mydata.dir/progress.make

# Include the compile flags for this target's objects.
include proto/CMakeFiles/mydata.dir/flags.make

proto/mydata.pb.h: ../proto/mydata.proto
proto/mydata.pb.h: /usr/local/bin/protoc-3.19.4.0
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ddming/Study/cmake/demo2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Running cpp protocol buffer compiler on mydata.proto. Custom options: "
	cd /home/ddming/Study/cmake/demo2/build/proto && /usr/local/bin/protoc-3.19.4.0 --cpp_out /home/ddming/Study/cmake/demo2/build/proto -I /home/ddming/Study/cmake/demo2/proto /home/ddming/Study/cmake/demo2/proto/mydata.proto

proto/mydata.pb.cc: proto/mydata.pb.h
	@$(CMAKE_COMMAND) -E touch_nocreate proto/mydata.pb.cc

proto/CMakeFiles/mydata.dir/mydata.pb.cc.o: proto/CMakeFiles/mydata.dir/flags.make
proto/CMakeFiles/mydata.dir/mydata.pb.cc.o: proto/mydata.pb.cc
proto/CMakeFiles/mydata.dir/mydata.pb.cc.o: proto/CMakeFiles/mydata.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ddming/Study/cmake/demo2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object proto/CMakeFiles/mydata.dir/mydata.pb.cc.o"
	cd /home/ddming/Study/cmake/demo2/build/proto && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT proto/CMakeFiles/mydata.dir/mydata.pb.cc.o -MF CMakeFiles/mydata.dir/mydata.pb.cc.o.d -o CMakeFiles/mydata.dir/mydata.pb.cc.o -c /home/ddming/Study/cmake/demo2/build/proto/mydata.pb.cc

proto/CMakeFiles/mydata.dir/mydata.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mydata.dir/mydata.pb.cc.i"
	cd /home/ddming/Study/cmake/demo2/build/proto && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ddming/Study/cmake/demo2/build/proto/mydata.pb.cc > CMakeFiles/mydata.dir/mydata.pb.cc.i

proto/CMakeFiles/mydata.dir/mydata.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mydata.dir/mydata.pb.cc.s"
	cd /home/ddming/Study/cmake/demo2/build/proto && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ddming/Study/cmake/demo2/build/proto/mydata.pb.cc -o CMakeFiles/mydata.dir/mydata.pb.cc.s

# Object files for target mydata
mydata_OBJECTS = \
"CMakeFiles/mydata.dir/mydata.pb.cc.o"

# External object files for target mydata
mydata_EXTERNAL_OBJECTS =

proto/libmydata.a: proto/CMakeFiles/mydata.dir/mydata.pb.cc.o
proto/libmydata.a: proto/CMakeFiles/mydata.dir/build.make
proto/libmydata.a: proto/CMakeFiles/mydata.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ddming/Study/cmake/demo2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libmydata.a"
	cd /home/ddming/Study/cmake/demo2/build/proto && $(CMAKE_COMMAND) -P CMakeFiles/mydata.dir/cmake_clean_target.cmake
	cd /home/ddming/Study/cmake/demo2/build/proto && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mydata.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
proto/CMakeFiles/mydata.dir/build: proto/libmydata.a
.PHONY : proto/CMakeFiles/mydata.dir/build

proto/CMakeFiles/mydata.dir/clean:
	cd /home/ddming/Study/cmake/demo2/build/proto && $(CMAKE_COMMAND) -P CMakeFiles/mydata.dir/cmake_clean.cmake
.PHONY : proto/CMakeFiles/mydata.dir/clean

proto/CMakeFiles/mydata.dir/depend: proto/mydata.pb.cc
proto/CMakeFiles/mydata.dir/depend: proto/mydata.pb.h
	cd /home/ddming/Study/cmake/demo2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ddming/Study/cmake/demo2 /home/ddming/Study/cmake/demo2/proto /home/ddming/Study/cmake/demo2/build /home/ddming/Study/cmake/demo2/build/proto /home/ddming/Study/cmake/demo2/build/proto/CMakeFiles/mydata.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : proto/CMakeFiles/mydata.dir/depend

