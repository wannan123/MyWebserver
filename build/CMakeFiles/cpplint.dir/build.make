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

# Utility rule file for cpplint.

# Include the progress variables for this target.
include CMakeFiles/cpplint.dir/progress.make

CMakeFiles/cpplint:
	echo '/home/wuchengen/Desktop/MyWebserver/src/Acceptor.cpp /home/wuchengen/Desktop/MyWebserver/src/Buffer.cpp /home/wuchengen/Desktop/MyWebserver/src/Channel.cpp /home/wuchengen/Desktop/MyWebserver/src/Connection.cpp /home/wuchengen/Desktop/MyWebserver/src/Eventloop.cpp /home/wuchengen/Desktop/MyWebserver/src/InetAddress.cpp /home/wuchengen/Desktop/MyWebserver/src/Poller.cpp /home/wuchengen/Desktop/MyWebserver/src/Server.cpp /home/wuchengen/Desktop/MyWebserver/src/Socket.cpp /home/wuchengen/Desktop/MyWebserver/src/ThreadPool.cpp /home/wuchengen/Desktop/MyWebserver/src/include/Acceptor.h /home/wuchengen/Desktop/MyWebserver/src/include/Buffer.h /home/wuchengen/Desktop/MyWebserver/src/include/Channel.h /home/wuchengen/Desktop/MyWebserver/src/include/Connection.h /home/wuchengen/Desktop/MyWebserver/src/include/Eventloop.h /home/wuchengen/Desktop/MyWebserver/src/include/InetAddress.h /home/wuchengen/Desktop/MyWebserver/src/include/Marcros.h /home/wuchengen/Desktop/MyWebserver/src/include/Poller.h /home/wuchengen/Desktop/MyWebserver/src/include/Server.h /home/wuchengen/Desktop/MyWebserver/src/include/Socket.h /home/wuchengen/Desktop/MyWebserver/src/include/ThreadPool.h /home/wuchengen/Desktop/MyWebserver/src/include/util.h /home/wuchengen/Desktop/MyWebserver/src/util.cpp /home/wuchengen/Desktop/MyWebserver/test/chat_client.cpp /home/wuchengen/Desktop/MyWebserver/test/chat_server.cpp /home/wuchengen/Desktop/MyWebserver/test/echo_client.cpp /home/wuchengen/Desktop/MyWebserver/test/echo_clients.cpp /home/wuchengen/Desktop/MyWebserver/test/echo_server.cpp /home/wuchengen/Desktop/MyWebserver/test/thread_test.cpp' | xargs -n12 -P8 /home/wuchengen/Desktop/MyWebserver/build_support/cpplint.py --verbose=2 --quiet --linelength=120 --filter=-legal/copyright,-build/include_subdir,-readability/casting

cpplint: CMakeFiles/cpplint
cpplint: CMakeFiles/cpplint.dir/build.make

.PHONY : cpplint

# Rule to build all files generated by this target.
CMakeFiles/cpplint.dir/build: cpplint

.PHONY : CMakeFiles/cpplint.dir/build

CMakeFiles/cpplint.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cpplint.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cpplint.dir/clean

CMakeFiles/cpplint.dir/depend:
	cd /home/wuchengen/Desktop/MyWebserver/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wuchengen/Desktop/MyWebserver /home/wuchengen/Desktop/MyWebserver /home/wuchengen/Desktop/MyWebserver/build /home/wuchengen/Desktop/MyWebserver/build /home/wuchengen/Desktop/MyWebserver/build/CMakeFiles/cpplint.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cpplint.dir/depend

