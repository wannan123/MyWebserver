# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/wuchengen/Desktop/MyWebserver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wuchengen/Desktop/MyWebserver/build

# Include any dependencies generated for this target.
include test/CMakeFiles/chat_client.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/chat_client.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/chat_client.dir/flags.make

test/CMakeFiles/chat_client.dir/chat_client.cpp.o: test/CMakeFiles/chat_client.dir/flags.make
test/CMakeFiles/chat_client.dir/chat_client.cpp.o: ../test/chat_client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wuchengen/Desktop/MyWebserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/chat_client.dir/chat_client.cpp.o"
	cd /home/wuchengen/Desktop/MyWebserver/build/test && /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chat_client.dir/chat_client.cpp.o -c /home/wuchengen/Desktop/MyWebserver/test/chat_client.cpp

test/CMakeFiles/chat_client.dir/chat_client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chat_client.dir/chat_client.cpp.i"
	cd /home/wuchengen/Desktop/MyWebserver/build/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wuchengen/Desktop/MyWebserver/test/chat_client.cpp > CMakeFiles/chat_client.dir/chat_client.cpp.i

test/CMakeFiles/chat_client.dir/chat_client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chat_client.dir/chat_client.cpp.s"
	cd /home/wuchengen/Desktop/MyWebserver/build/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wuchengen/Desktop/MyWebserver/test/chat_client.cpp -o CMakeFiles/chat_client.dir/chat_client.cpp.s

# Object files for target chat_client
chat_client_OBJECTS = \
"CMakeFiles/chat_client.dir/chat_client.cpp.o"

# External object files for target chat_client
chat_client_EXTERNAL_OBJECTS =

bin/chat_client: test/CMakeFiles/chat_client.dir/chat_client.cpp.o
bin/chat_client: test/CMakeFiles/chat_client.dir/build.make
bin/chat_client: lib/libpine_shared.so
bin/chat_client: test/CMakeFiles/chat_client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wuchengen/Desktop/MyWebserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/chat_client"
	cd /home/wuchengen/Desktop/MyWebserver/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/chat_client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/chat_client.dir/build: bin/chat_client

.PHONY : test/CMakeFiles/chat_client.dir/build

test/CMakeFiles/chat_client.dir/clean:
	cd /home/wuchengen/Desktop/MyWebserver/build/test && $(CMAKE_COMMAND) -P CMakeFiles/chat_client.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/chat_client.dir/clean

test/CMakeFiles/chat_client.dir/depend:
	cd /home/wuchengen/Desktop/MyWebserver/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wuchengen/Desktop/MyWebserver /home/wuchengen/Desktop/MyWebserver/test /home/wuchengen/Desktop/MyWebserver/build /home/wuchengen/Desktop/MyWebserver/build/test /home/wuchengen/Desktop/MyWebserver/build/test/CMakeFiles/chat_client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/chat_client.dir/depend
