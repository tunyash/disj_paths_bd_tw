# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\Kirill\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\202.6397.106\bin\cmake\win\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\Kirill\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\202.6397.106\bin\cmake\win\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Kirill\CLionProjects\Treewidth

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Kirill\CLionProjects\Treewidth\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/nice_path_dec.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/nice_path_dec.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/nice_path_dec.dir/flags.make

CMakeFiles/nice_path_dec.dir/src/nice-path-decomposition/nice-path-decomposition.cpp.obj: CMakeFiles/nice_path_dec.dir/flags.make
CMakeFiles/nice_path_dec.dir/src/nice-path-decomposition/nice-path-decomposition.cpp.obj: CMakeFiles/nice_path_dec.dir/includes_CXX.rsp
CMakeFiles/nice_path_dec.dir/src/nice-path-decomposition/nice-path-decomposition.cpp.obj: ../src/nice-path-decomposition/nice-path-decomposition.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Kirill\CLionProjects\Treewidth\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/nice_path_dec.dir/src/nice-path-decomposition/nice-path-decomposition.cpp.obj"
	C:\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\nice_path_dec.dir\src\nice-path-decomposition\nice-path-decomposition.cpp.obj -c C:\Users\Kirill\CLionProjects\Treewidth\src\nice-path-decomposition\nice-path-decomposition.cpp

CMakeFiles/nice_path_dec.dir/src/nice-path-decomposition/nice-path-decomposition.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/nice_path_dec.dir/src/nice-path-decomposition/nice-path-decomposition.cpp.i"
	C:\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Kirill\CLionProjects\Treewidth\src\nice-path-decomposition\nice-path-decomposition.cpp > CMakeFiles\nice_path_dec.dir\src\nice-path-decomposition\nice-path-decomposition.cpp.i

CMakeFiles/nice_path_dec.dir/src/nice-path-decomposition/nice-path-decomposition.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/nice_path_dec.dir/src/nice-path-decomposition/nice-path-decomposition.cpp.s"
	C:\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Kirill\CLionProjects\Treewidth\src\nice-path-decomposition\nice-path-decomposition.cpp -o CMakeFiles\nice_path_dec.dir\src\nice-path-decomposition\nice-path-decomposition.cpp.s

# Object files for target nice_path_dec
nice_path_dec_OBJECTS = \
"CMakeFiles/nice_path_dec.dir/src/nice-path-decomposition/nice-path-decomposition.cpp.obj"

# External object files for target nice_path_dec
nice_path_dec_EXTERNAL_OBJECTS =

libnice_path_dec.a: CMakeFiles/nice_path_dec.dir/src/nice-path-decomposition/nice-path-decomposition.cpp.obj
libnice_path_dec.a: CMakeFiles/nice_path_dec.dir/build.make
libnice_path_dec.a: CMakeFiles/nice_path_dec.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Kirill\CLionProjects\Treewidth\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libnice_path_dec.a"
	$(CMAKE_COMMAND) -P CMakeFiles\nice_path_dec.dir\cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\nice_path_dec.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/nice_path_dec.dir/build: libnice_path_dec.a

.PHONY : CMakeFiles/nice_path_dec.dir/build

CMakeFiles/nice_path_dec.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\nice_path_dec.dir\cmake_clean.cmake
.PHONY : CMakeFiles/nice_path_dec.dir/clean

CMakeFiles/nice_path_dec.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Kirill\CLionProjects\Treewidth C:\Users\Kirill\CLionProjects\Treewidth C:\Users\Kirill\CLionProjects\Treewidth\cmake-build-debug C:\Users\Kirill\CLionProjects\Treewidth\cmake-build-debug C:\Users\Kirill\CLionProjects\Treewidth\cmake-build-debug\CMakeFiles\nice_path_dec.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/nice_path_dec.dir/depend

