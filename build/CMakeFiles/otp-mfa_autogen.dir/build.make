# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/oorlovsk/Downloads/otp-mfa-fisrt

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/oorlovsk/Downloads/otp-mfa-fisrt/build

# Utility rule file for otp-mfa_autogen.

# Include any custom commands dependencies for this target.
include CMakeFiles/otp-mfa_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/otp-mfa_autogen.dir/progress.make

CMakeFiles/otp-mfa_autogen: otp-mfa_autogen/timestamp

otp-mfa_autogen/timestamp: /usr/lib/qt5/bin/moc
otp-mfa_autogen/timestamp: /usr/lib/qt5/bin/uic
otp-mfa_autogen/timestamp: CMakeFiles/otp-mfa_autogen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/oorlovsk/Downloads/otp-mfa-fisrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target otp-mfa"
	/usr/bin/cmake -E cmake_autogen /home/oorlovsk/Downloads/otp-mfa-fisrt/build/CMakeFiles/otp-mfa_autogen.dir/AutogenInfo.json Release
	/usr/bin/cmake -E touch /home/oorlovsk/Downloads/otp-mfa-fisrt/build/otp-mfa_autogen/timestamp

otp-mfa_autogen: CMakeFiles/otp-mfa_autogen
otp-mfa_autogen: otp-mfa_autogen/timestamp
otp-mfa_autogen: CMakeFiles/otp-mfa_autogen.dir/build.make
.PHONY : otp-mfa_autogen

# Rule to build all files generated by this target.
CMakeFiles/otp-mfa_autogen.dir/build: otp-mfa_autogen
.PHONY : CMakeFiles/otp-mfa_autogen.dir/build

CMakeFiles/otp-mfa_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/otp-mfa_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/otp-mfa_autogen.dir/clean

CMakeFiles/otp-mfa_autogen.dir/depend:
	cd /home/oorlovsk/Downloads/otp-mfa-fisrt/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/oorlovsk/Downloads/otp-mfa-fisrt /home/oorlovsk/Downloads/otp-mfa-fisrt /home/oorlovsk/Downloads/otp-mfa-fisrt/build /home/oorlovsk/Downloads/otp-mfa-fisrt/build /home/oorlovsk/Downloads/otp-mfa-fisrt/build/CMakeFiles/otp-mfa_autogen.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/otp-mfa_autogen.dir/depend

