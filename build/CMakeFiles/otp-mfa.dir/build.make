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
CMAKE_SOURCE_DIR = /home/oorlovsk/otp-mfa

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/oorlovsk/otp-mfa/build

# Include any dependencies generated for this target.
include CMakeFiles/otp-mfa.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/otp-mfa.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/otp-mfa.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/otp-mfa.dir/flags.make

otp-mfa_autogen/timestamp: /usr/lib/qt6/libexec/moc
otp-mfa_autogen/timestamp: /usr/lib/qt6/libexec/uic
otp-mfa_autogen/timestamp: CMakeFiles/otp-mfa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/oorlovsk/otp-mfa/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target otp-mfa"
	/usr/bin/cmake -E cmake_autogen /home/oorlovsk/otp-mfa/build/CMakeFiles/otp-mfa_autogen.dir/AutogenInfo.json ""
	/usr/bin/cmake -E touch /home/oorlovsk/otp-mfa/build/otp-mfa_autogen/timestamp

CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.o: CMakeFiles/otp-mfa.dir/flags.make
CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.o: otp-mfa_autogen/mocs_compilation.cpp
CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.o: CMakeFiles/otp-mfa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/oorlovsk/otp-mfa/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.o -MF CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.o.d -o CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.o -c /home/oorlovsk/otp-mfa/build/otp-mfa_autogen/mocs_compilation.cpp

CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oorlovsk/otp-mfa/build/otp-mfa_autogen/mocs_compilation.cpp > CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.i

CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oorlovsk/otp-mfa/build/otp-mfa_autogen/mocs_compilation.cpp -o CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.s

CMakeFiles/otp-mfa.dir/main.cpp.o: CMakeFiles/otp-mfa.dir/flags.make
CMakeFiles/otp-mfa.dir/main.cpp.o: /home/oorlovsk/otp-mfa/main.cpp
CMakeFiles/otp-mfa.dir/main.cpp.o: CMakeFiles/otp-mfa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/oorlovsk/otp-mfa/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/otp-mfa.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/otp-mfa.dir/main.cpp.o -MF CMakeFiles/otp-mfa.dir/main.cpp.o.d -o CMakeFiles/otp-mfa.dir/main.cpp.o -c /home/oorlovsk/otp-mfa/main.cpp

CMakeFiles/otp-mfa.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/otp-mfa.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oorlovsk/otp-mfa/main.cpp > CMakeFiles/otp-mfa.dir/main.cpp.i

CMakeFiles/otp-mfa.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/otp-mfa.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oorlovsk/otp-mfa/main.cpp -o CMakeFiles/otp-mfa.dir/main.cpp.s

CMakeFiles/otp-mfa.dir/otp_window.cpp.o: CMakeFiles/otp-mfa.dir/flags.make
CMakeFiles/otp-mfa.dir/otp_window.cpp.o: /home/oorlovsk/otp-mfa/otp_window.cpp
CMakeFiles/otp-mfa.dir/otp_window.cpp.o: CMakeFiles/otp-mfa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/oorlovsk/otp-mfa/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/otp-mfa.dir/otp_window.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/otp-mfa.dir/otp_window.cpp.o -MF CMakeFiles/otp-mfa.dir/otp_window.cpp.o.d -o CMakeFiles/otp-mfa.dir/otp_window.cpp.o -c /home/oorlovsk/otp-mfa/otp_window.cpp

CMakeFiles/otp-mfa.dir/otp_window.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/otp-mfa.dir/otp_window.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oorlovsk/otp-mfa/otp_window.cpp > CMakeFiles/otp-mfa.dir/otp_window.cpp.i

CMakeFiles/otp-mfa.dir/otp_window.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/otp-mfa.dir/otp_window.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oorlovsk/otp-mfa/otp_window.cpp -o CMakeFiles/otp-mfa.dir/otp_window.cpp.s

CMakeFiles/otp-mfa.dir/otp_generator.cpp.o: CMakeFiles/otp-mfa.dir/flags.make
CMakeFiles/otp-mfa.dir/otp_generator.cpp.o: /home/oorlovsk/otp-mfa/otp_generator.cpp
CMakeFiles/otp-mfa.dir/otp_generator.cpp.o: CMakeFiles/otp-mfa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/oorlovsk/otp-mfa/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/otp-mfa.dir/otp_generator.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/otp-mfa.dir/otp_generator.cpp.o -MF CMakeFiles/otp-mfa.dir/otp_generator.cpp.o.d -o CMakeFiles/otp-mfa.dir/otp_generator.cpp.o -c /home/oorlovsk/otp-mfa/otp_generator.cpp

CMakeFiles/otp-mfa.dir/otp_generator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/otp-mfa.dir/otp_generator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oorlovsk/otp-mfa/otp_generator.cpp > CMakeFiles/otp-mfa.dir/otp_generator.cpp.i

CMakeFiles/otp-mfa.dir/otp_generator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/otp-mfa.dir/otp_generator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oorlovsk/otp-mfa/otp_generator.cpp -o CMakeFiles/otp-mfa.dir/otp_generator.cpp.s

CMakeFiles/otp-mfa.dir/account_manager.cpp.o: CMakeFiles/otp-mfa.dir/flags.make
CMakeFiles/otp-mfa.dir/account_manager.cpp.o: /home/oorlovsk/otp-mfa/account_manager.cpp
CMakeFiles/otp-mfa.dir/account_manager.cpp.o: CMakeFiles/otp-mfa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/oorlovsk/otp-mfa/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/otp-mfa.dir/account_manager.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/otp-mfa.dir/account_manager.cpp.o -MF CMakeFiles/otp-mfa.dir/account_manager.cpp.o.d -o CMakeFiles/otp-mfa.dir/account_manager.cpp.o -c /home/oorlovsk/otp-mfa/account_manager.cpp

CMakeFiles/otp-mfa.dir/account_manager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/otp-mfa.dir/account_manager.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oorlovsk/otp-mfa/account_manager.cpp > CMakeFiles/otp-mfa.dir/account_manager.cpp.i

CMakeFiles/otp-mfa.dir/account_manager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/otp-mfa.dir/account_manager.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oorlovsk/otp-mfa/account_manager.cpp -o CMakeFiles/otp-mfa.dir/account_manager.cpp.s

CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.o: CMakeFiles/otp-mfa.dir/flags.make
CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.o: /home/oorlovsk/otp-mfa/add_account_dialog.cpp
CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.o: CMakeFiles/otp-mfa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/oorlovsk/otp-mfa/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.o -MF CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.o.d -o CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.o -c /home/oorlovsk/otp-mfa/add_account_dialog.cpp

CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oorlovsk/otp-mfa/add_account_dialog.cpp > CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.i

CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oorlovsk/otp-mfa/add_account_dialog.cpp -o CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.s

# Object files for target otp-mfa
otp__mfa_OBJECTS = \
"CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/otp-mfa.dir/main.cpp.o" \
"CMakeFiles/otp-mfa.dir/otp_window.cpp.o" \
"CMakeFiles/otp-mfa.dir/otp_generator.cpp.o" \
"CMakeFiles/otp-mfa.dir/account_manager.cpp.o" \
"CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.o"

# External object files for target otp-mfa
otp__mfa_EXTERNAL_OBJECTS =

otp-mfa: CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.o
otp-mfa: CMakeFiles/otp-mfa.dir/main.cpp.o
otp-mfa: CMakeFiles/otp-mfa.dir/otp_window.cpp.o
otp-mfa: CMakeFiles/otp-mfa.dir/otp_generator.cpp.o
otp-mfa: CMakeFiles/otp-mfa.dir/account_manager.cpp.o
otp-mfa: CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.o
otp-mfa: CMakeFiles/otp-mfa.dir/build.make
otp-mfa: /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.4.2
otp-mfa: /usr/lib/x86_64-linux-gnu/libssl.so
otp-mfa: /usr/lib/x86_64-linux-gnu/libcrypto.so
otp-mfa: /usr/lib/x86_64-linux-gnu/libQt6Gui.so.6.4.2
otp-mfa: /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.4.2
otp-mfa: /usr/lib/x86_64-linux-gnu/libGLX.so
otp-mfa: /usr/lib/x86_64-linux-gnu/libOpenGL.so
otp-mfa: CMakeFiles/otp-mfa.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/oorlovsk/otp-mfa/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable otp-mfa"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/otp-mfa.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/otp-mfa.dir/build: otp-mfa
.PHONY : CMakeFiles/otp-mfa.dir/build

CMakeFiles/otp-mfa.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/otp-mfa.dir/cmake_clean.cmake
.PHONY : CMakeFiles/otp-mfa.dir/clean

CMakeFiles/otp-mfa.dir/depend: otp-mfa_autogen/timestamp
	cd /home/oorlovsk/otp-mfa/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/oorlovsk/otp-mfa /home/oorlovsk/otp-mfa /home/oorlovsk/otp-mfa/build /home/oorlovsk/otp-mfa/build /home/oorlovsk/otp-mfa/build/CMakeFiles/otp-mfa.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/otp-mfa.dir/depend

