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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/cristian.aguilar596/Desktop/comp232/ciLisp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/cristian.aguilar596/Desktop/comp232/ciLisp/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/cilisp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/cilisp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cilisp.dir/flags.make

ciLispParser.c: ../src/ciLisp.y
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/cristian.aguilar596/Desktop/comp232/ciLisp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "[BISON][ciLispParser] Building parser with bison 2.3"
	cd /Users/cristian.aguilar596/Desktop/comp232/ciLisp && /usr/bin/bison -d --verbose -o /Users/cristian.aguilar596/Desktop/comp232/ciLisp/cmake-build-debug/ciLispParser.c src/ciLisp.y

ciLispParser.output: ciLispParser.c
	@$(CMAKE_COMMAND) -E touch_nocreate ciLispParser.output

ciLispParser.h: ciLispParser.c
	@$(CMAKE_COMMAND) -E touch_nocreate ciLispParser.h

ciLispScanner.c: ../src/ciLisp.l
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/cristian.aguilar596/Desktop/comp232/ciLisp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "[FLEX][ciLispScanner] Building scanner with flex 2.5.35"
	cd /Users/cristian.aguilar596/Desktop/comp232/ciLisp && /usr/bin/flex -o/Users/cristian.aguilar596/Desktop/comp232/ciLisp/cmake-build-debug/ciLispScanner.c src/ciLisp.l

CMakeFiles/cilisp.dir/src/ciLisp.c.o: CMakeFiles/cilisp.dir/flags.make
CMakeFiles/cilisp.dir/src/ciLisp.c.o: ../src/ciLisp.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/cristian.aguilar596/Desktop/comp232/ciLisp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/cilisp.dir/src/ciLisp.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/cilisp.dir/src/ciLisp.c.o   -c /Users/cristian.aguilar596/Desktop/comp232/ciLisp/src/ciLisp.c

CMakeFiles/cilisp.dir/src/ciLisp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cilisp.dir/src/ciLisp.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/cristian.aguilar596/Desktop/comp232/ciLisp/src/ciLisp.c > CMakeFiles/cilisp.dir/src/ciLisp.c.i

CMakeFiles/cilisp.dir/src/ciLisp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cilisp.dir/src/ciLisp.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/cristian.aguilar596/Desktop/comp232/ciLisp/src/ciLisp.c -o CMakeFiles/cilisp.dir/src/ciLisp.c.s

CMakeFiles/cilisp.dir/ciLispScanner.c.o: CMakeFiles/cilisp.dir/flags.make
CMakeFiles/cilisp.dir/ciLispScanner.c.o: ciLispScanner.c
CMakeFiles/cilisp.dir/ciLispScanner.c.o: ciLispParser.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/cristian.aguilar596/Desktop/comp232/ciLisp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/cilisp.dir/ciLispScanner.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/cilisp.dir/ciLispScanner.c.o   -c /Users/cristian.aguilar596/Desktop/comp232/ciLisp/cmake-build-debug/ciLispScanner.c

CMakeFiles/cilisp.dir/ciLispScanner.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cilisp.dir/ciLispScanner.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/cristian.aguilar596/Desktop/comp232/ciLisp/cmake-build-debug/ciLispScanner.c > CMakeFiles/cilisp.dir/ciLispScanner.c.i

CMakeFiles/cilisp.dir/ciLispScanner.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cilisp.dir/ciLispScanner.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/cristian.aguilar596/Desktop/comp232/ciLisp/cmake-build-debug/ciLispScanner.c -o CMakeFiles/cilisp.dir/ciLispScanner.c.s

CMakeFiles/cilisp.dir/ciLispParser.c.o: CMakeFiles/cilisp.dir/flags.make
CMakeFiles/cilisp.dir/ciLispParser.c.o: ciLispParser.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/cristian.aguilar596/Desktop/comp232/ciLisp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/cilisp.dir/ciLispParser.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/cilisp.dir/ciLispParser.c.o   -c /Users/cristian.aguilar596/Desktop/comp232/ciLisp/cmake-build-debug/ciLispParser.c

CMakeFiles/cilisp.dir/ciLispParser.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cilisp.dir/ciLispParser.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/cristian.aguilar596/Desktop/comp232/ciLisp/cmake-build-debug/ciLispParser.c > CMakeFiles/cilisp.dir/ciLispParser.c.i

CMakeFiles/cilisp.dir/ciLispParser.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cilisp.dir/ciLispParser.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/cristian.aguilar596/Desktop/comp232/ciLisp/cmake-build-debug/ciLispParser.c -o CMakeFiles/cilisp.dir/ciLispParser.c.s

# Object files for target cilisp
cilisp_OBJECTS = \
"CMakeFiles/cilisp.dir/src/ciLisp.c.o" \
"CMakeFiles/cilisp.dir/ciLispScanner.c.o" \
"CMakeFiles/cilisp.dir/ciLispParser.c.o"

# External object files for target cilisp
cilisp_EXTERNAL_OBJECTS =

cilisp: CMakeFiles/cilisp.dir/src/ciLisp.c.o
cilisp: CMakeFiles/cilisp.dir/ciLispScanner.c.o
cilisp: CMakeFiles/cilisp.dir/ciLispParser.c.o
cilisp: CMakeFiles/cilisp.dir/build.make
cilisp: CMakeFiles/cilisp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/cristian.aguilar596/Desktop/comp232/ciLisp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable cilisp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cilisp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cilisp.dir/build: cilisp

.PHONY : CMakeFiles/cilisp.dir/build

CMakeFiles/cilisp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cilisp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cilisp.dir/clean

CMakeFiles/cilisp.dir/depend: ciLispParser.c
CMakeFiles/cilisp.dir/depend: ciLispParser.output
CMakeFiles/cilisp.dir/depend: ciLispParser.h
CMakeFiles/cilisp.dir/depend: ciLispScanner.c
	cd /Users/cristian.aguilar596/Desktop/comp232/ciLisp/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/cristian.aguilar596/Desktop/comp232/ciLisp /Users/cristian.aguilar596/Desktop/comp232/ciLisp /Users/cristian.aguilar596/Desktop/comp232/ciLisp/cmake-build-debug /Users/cristian.aguilar596/Desktop/comp232/ciLisp/cmake-build-debug /Users/cristian.aguilar596/Desktop/comp232/ciLisp/cmake-build-debug/CMakeFiles/cilisp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cilisp.dir/depend
