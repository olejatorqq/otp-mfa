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

# Include any dependencies generated for this target.
include CMakeFiles/otp-mfa.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/otp-mfa.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/otp-mfa.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/otp-mfa.dir/flags.make

otp-mfa_autogen/timestamp: /usr/lib/qt5/bin/moc
otp-mfa_autogen/timestamp: /usr/lib/qt5/bin/uic
otp-mfa_autogen/timestamp: CMakeFiles/otp-mfa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/oorlovsk/Downloads/otp-mfa-fisrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target otp-mfa"
	/usr/bin/cmake -E cmake_autogen /home/oorlovsk/Downloads/otp-mfa-fisrt/build/CMakeFiles/otp-mfa_autogen.dir/AutogenInfo.json ""
	/usr/bin/cmake -E touch /home/oorlovsk/Downloads/otp-mfa-fisrt/build/otp-mfa_autogen/timestamp

CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.o: CMakeFiles/otp-mfa.dir/flags.make
CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.o: otp-mfa_autogen/mocs_compilation.cpp
CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.o: CMakeFiles/otp-mfa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/oorlovsk/Downloads/otp-mfa-fisrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.o -MF CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.o.d -o CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.o -c /home/oorlovsk/Downloads/otp-mfa-fisrt/build/otp-mfa_autogen/mocs_compilation.cpp

CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oorlovsk/Downloads/otp-mfa-fisrt/build/otp-mfa_autogen/mocs_compilation.cpp > CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.i

CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oorlovsk/Downloads/otp-mfa-fisrt/build/otp-mfa_autogen/mocs_compilation.cpp -o CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.s

CMakeFiles/otp-mfa.dir/main.cpp.o: CMakeFiles/otp-mfa.dir/flags.make
CMakeFiles/otp-mfa.dir/main.cpp.o: /home/oorlovsk/Downloads/otp-mfa-fisrt/main.cpp
CMakeFiles/otp-mfa.dir/main.cpp.o: CMakeFiles/otp-mfa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/oorlovsk/Downloads/otp-mfa-fisrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/otp-mfa.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/otp-mfa.dir/main.cpp.o -MF CMakeFiles/otp-mfa.dir/main.cpp.o.d -o CMakeFiles/otp-mfa.dir/main.cpp.o -c /home/oorlovsk/Downloads/otp-mfa-fisrt/main.cpp

CMakeFiles/otp-mfa.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/otp-mfa.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oorlovsk/Downloads/otp-mfa-fisrt/main.cpp > CMakeFiles/otp-mfa.dir/main.cpp.i

CMakeFiles/otp-mfa.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/otp-mfa.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oorlovsk/Downloads/otp-mfa-fisrt/main.cpp -o CMakeFiles/otp-mfa.dir/main.cpp.s

CMakeFiles/otp-mfa.dir/otp_window.cpp.o: CMakeFiles/otp-mfa.dir/flags.make
CMakeFiles/otp-mfa.dir/otp_window.cpp.o: /home/oorlovsk/Downloads/otp-mfa-fisrt/otp_window.cpp
CMakeFiles/otp-mfa.dir/otp_window.cpp.o: CMakeFiles/otp-mfa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/oorlovsk/Downloads/otp-mfa-fisrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/otp-mfa.dir/otp_window.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/otp-mfa.dir/otp_window.cpp.o -MF CMakeFiles/otp-mfa.dir/otp_window.cpp.o.d -o CMakeFiles/otp-mfa.dir/otp_window.cpp.o -c /home/oorlovsk/Downloads/otp-mfa-fisrt/otp_window.cpp

CMakeFiles/otp-mfa.dir/otp_window.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/otp-mfa.dir/otp_window.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oorlovsk/Downloads/otp-mfa-fisrt/otp_window.cpp > CMakeFiles/otp-mfa.dir/otp_window.cpp.i

CMakeFiles/otp-mfa.dir/otp_window.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/otp-mfa.dir/otp_window.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oorlovsk/Downloads/otp-mfa-fisrt/otp_window.cpp -o CMakeFiles/otp-mfa.dir/otp_window.cpp.s

CMakeFiles/otp-mfa.dir/otp_generator.cpp.o: CMakeFiles/otp-mfa.dir/flags.make
CMakeFiles/otp-mfa.dir/otp_generator.cpp.o: /home/oorlovsk/Downloads/otp-mfa-fisrt/otp_generator.cpp
CMakeFiles/otp-mfa.dir/otp_generator.cpp.o: CMakeFiles/otp-mfa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/oorlovsk/Downloads/otp-mfa-fisrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/otp-mfa.dir/otp_generator.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/otp-mfa.dir/otp_generator.cpp.o -MF CMakeFiles/otp-mfa.dir/otp_generator.cpp.o.d -o CMakeFiles/otp-mfa.dir/otp_generator.cpp.o -c /home/oorlovsk/Downloads/otp-mfa-fisrt/otp_generator.cpp

CMakeFiles/otp-mfa.dir/otp_generator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/otp-mfa.dir/otp_generator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oorlovsk/Downloads/otp-mfa-fisrt/otp_generator.cpp > CMakeFiles/otp-mfa.dir/otp_generator.cpp.i

CMakeFiles/otp-mfa.dir/otp_generator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/otp-mfa.dir/otp_generator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oorlovsk/Downloads/otp-mfa-fisrt/otp_generator.cpp -o CMakeFiles/otp-mfa.dir/otp_generator.cpp.s

CMakeFiles/otp-mfa.dir/account_manager.cpp.o: CMakeFiles/otp-mfa.dir/flags.make
CMakeFiles/otp-mfa.dir/account_manager.cpp.o: /home/oorlovsk/Downloads/otp-mfa-fisrt/account_manager.cpp
CMakeFiles/otp-mfa.dir/account_manager.cpp.o: CMakeFiles/otp-mfa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/oorlovsk/Downloads/otp-mfa-fisrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/otp-mfa.dir/account_manager.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/otp-mfa.dir/account_manager.cpp.o -MF CMakeFiles/otp-mfa.dir/account_manager.cpp.o.d -o CMakeFiles/otp-mfa.dir/account_manager.cpp.o -c /home/oorlovsk/Downloads/otp-mfa-fisrt/account_manager.cpp

CMakeFiles/otp-mfa.dir/account_manager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/otp-mfa.dir/account_manager.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oorlovsk/Downloads/otp-mfa-fisrt/account_manager.cpp > CMakeFiles/otp-mfa.dir/account_manager.cpp.i

CMakeFiles/otp-mfa.dir/account_manager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/otp-mfa.dir/account_manager.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oorlovsk/Downloads/otp-mfa-fisrt/account_manager.cpp -o CMakeFiles/otp-mfa.dir/account_manager.cpp.s

CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.o: CMakeFiles/otp-mfa.dir/flags.make
CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.o: /home/oorlovsk/Downloads/otp-mfa-fisrt/add_account_dialog.cpp
CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.o: CMakeFiles/otp-mfa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/oorlovsk/Downloads/otp-mfa-fisrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.o -MF CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.o.d -o CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.o -c /home/oorlovsk/Downloads/otp-mfa-fisrt/add_account_dialog.cpp

CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oorlovsk/Downloads/otp-mfa-fisrt/add_account_dialog.cpp > CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.i

CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oorlovsk/Downloads/otp-mfa-fisrt/add_account_dialog.cpp -o CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.s

CMakeFiles/otp-mfa.dir/qr_generator.cpp.o: CMakeFiles/otp-mfa.dir/flags.make
CMakeFiles/otp-mfa.dir/qr_generator.cpp.o: /home/oorlovsk/Downloads/otp-mfa-fisrt/qr_generator.cpp
CMakeFiles/otp-mfa.dir/qr_generator.cpp.o: CMakeFiles/otp-mfa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/oorlovsk/Downloads/otp-mfa-fisrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/otp-mfa.dir/qr_generator.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/otp-mfa.dir/qr_generator.cpp.o -MF CMakeFiles/otp-mfa.dir/qr_generator.cpp.o.d -o CMakeFiles/otp-mfa.dir/qr_generator.cpp.o -c /home/oorlovsk/Downloads/otp-mfa-fisrt/qr_generator.cpp

CMakeFiles/otp-mfa.dir/qr_generator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/otp-mfa.dir/qr_generator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oorlovsk/Downloads/otp-mfa-fisrt/qr_generator.cpp > CMakeFiles/otp-mfa.dir/qr_generator.cpp.i

CMakeFiles/otp-mfa.dir/qr_generator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/otp-mfa.dir/qr_generator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oorlovsk/Downloads/otp-mfa-fisrt/qr_generator.cpp -o CMakeFiles/otp-mfa.dir/qr_generator.cpp.s

CMakeFiles/otp-mfa.dir/encryption_utils.cpp.o: CMakeFiles/otp-mfa.dir/flags.make
CMakeFiles/otp-mfa.dir/encryption_utils.cpp.o: /home/oorlovsk/Downloads/otp-mfa-fisrt/encryption_utils.cpp
CMakeFiles/otp-mfa.dir/encryption_utils.cpp.o: CMakeFiles/otp-mfa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/oorlovsk/Downloads/otp-mfa-fisrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/otp-mfa.dir/encryption_utils.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/otp-mfa.dir/encryption_utils.cpp.o -MF CMakeFiles/otp-mfa.dir/encryption_utils.cpp.o.d -o CMakeFiles/otp-mfa.dir/encryption_utils.cpp.o -c /home/oorlovsk/Downloads/otp-mfa-fisrt/encryption_utils.cpp

CMakeFiles/otp-mfa.dir/encryption_utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/otp-mfa.dir/encryption_utils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oorlovsk/Downloads/otp-mfa-fisrt/encryption_utils.cpp > CMakeFiles/otp-mfa.dir/encryption_utils.cpp.i

CMakeFiles/otp-mfa.dir/encryption_utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/otp-mfa.dir/encryption_utils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oorlovsk/Downloads/otp-mfa-fisrt/encryption_utils.cpp -o CMakeFiles/otp-mfa.dir/encryption_utils.cpp.s

CMakeFiles/otp-mfa.dir/master_password_dialog.cpp.o: CMakeFiles/otp-mfa.dir/flags.make
CMakeFiles/otp-mfa.dir/master_password_dialog.cpp.o: /home/oorlovsk/Downloads/otp-mfa-fisrt/master_password_dialog.cpp
CMakeFiles/otp-mfa.dir/master_password_dialog.cpp.o: CMakeFiles/otp-mfa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/oorlovsk/Downloads/otp-mfa-fisrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/otp-mfa.dir/master_password_dialog.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/otp-mfa.dir/master_password_dialog.cpp.o -MF CMakeFiles/otp-mfa.dir/master_password_dialog.cpp.o.d -o CMakeFiles/otp-mfa.dir/master_password_dialog.cpp.o -c /home/oorlovsk/Downloads/otp-mfa-fisrt/master_password_dialog.cpp

CMakeFiles/otp-mfa.dir/master_password_dialog.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/otp-mfa.dir/master_password_dialog.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oorlovsk/Downloads/otp-mfa-fisrt/master_password_dialog.cpp > CMakeFiles/otp-mfa.dir/master_password_dialog.cpp.i

CMakeFiles/otp-mfa.dir/master_password_dialog.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/otp-mfa.dir/master_password_dialog.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oorlovsk/Downloads/otp-mfa-fisrt/master_password_dialog.cpp -o CMakeFiles/otp-mfa.dir/master_password_dialog.cpp.s

CMakeFiles/otp-mfa.dir/settings_dialog.cpp.o: CMakeFiles/otp-mfa.dir/flags.make
CMakeFiles/otp-mfa.dir/settings_dialog.cpp.o: /home/oorlovsk/Downloads/otp-mfa-fisrt/settings_dialog.cpp
CMakeFiles/otp-mfa.dir/settings_dialog.cpp.o: CMakeFiles/otp-mfa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/oorlovsk/Downloads/otp-mfa-fisrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/otp-mfa.dir/settings_dialog.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/otp-mfa.dir/settings_dialog.cpp.o -MF CMakeFiles/otp-mfa.dir/settings_dialog.cpp.o.d -o CMakeFiles/otp-mfa.dir/settings_dialog.cpp.o -c /home/oorlovsk/Downloads/otp-mfa-fisrt/settings_dialog.cpp

CMakeFiles/otp-mfa.dir/settings_dialog.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/otp-mfa.dir/settings_dialog.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oorlovsk/Downloads/otp-mfa-fisrt/settings_dialog.cpp > CMakeFiles/otp-mfa.dir/settings_dialog.cpp.i

CMakeFiles/otp-mfa.dir/settings_dialog.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/otp-mfa.dir/settings_dialog.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oorlovsk/Downloads/otp-mfa-fisrt/settings_dialog.cpp -o CMakeFiles/otp-mfa.dir/settings_dialog.cpp.s

CMakeFiles/otp-mfa.dir/home/oorlovsk/otp-mfa/include/qrcodegen.cpp.o: CMakeFiles/otp-mfa.dir/flags.make
CMakeFiles/otp-mfa.dir/home/oorlovsk/otp-mfa/include/qrcodegen.cpp.o: /home/oorlovsk/otp-mfa/include/qrcodegen.cpp
CMakeFiles/otp-mfa.dir/home/oorlovsk/otp-mfa/include/qrcodegen.cpp.o: CMakeFiles/otp-mfa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/oorlovsk/Downloads/otp-mfa-fisrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/otp-mfa.dir/home/oorlovsk/otp-mfa/include/qrcodegen.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/otp-mfa.dir/home/oorlovsk/otp-mfa/include/qrcodegen.cpp.o -MF CMakeFiles/otp-mfa.dir/home/oorlovsk/otp-mfa/include/qrcodegen.cpp.o.d -o CMakeFiles/otp-mfa.dir/home/oorlovsk/otp-mfa/include/qrcodegen.cpp.o -c /home/oorlovsk/otp-mfa/include/qrcodegen.cpp

CMakeFiles/otp-mfa.dir/home/oorlovsk/otp-mfa/include/qrcodegen.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/otp-mfa.dir/home/oorlovsk/otp-mfa/include/qrcodegen.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oorlovsk/otp-mfa/include/qrcodegen.cpp > CMakeFiles/otp-mfa.dir/home/oorlovsk/otp-mfa/include/qrcodegen.cpp.i

CMakeFiles/otp-mfa.dir/home/oorlovsk/otp-mfa/include/qrcodegen.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/otp-mfa.dir/home/oorlovsk/otp-mfa/include/qrcodegen.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oorlovsk/otp-mfa/include/qrcodegen.cpp -o CMakeFiles/otp-mfa.dir/home/oorlovsk/otp-mfa/include/qrcodegen.cpp.s

CMakeFiles/otp-mfa.dir/screenshot_tool.cpp.o: CMakeFiles/otp-mfa.dir/flags.make
CMakeFiles/otp-mfa.dir/screenshot_tool.cpp.o: /home/oorlovsk/Downloads/otp-mfa-fisrt/screenshot_tool.cpp
CMakeFiles/otp-mfa.dir/screenshot_tool.cpp.o: CMakeFiles/otp-mfa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/oorlovsk/Downloads/otp-mfa-fisrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/otp-mfa.dir/screenshot_tool.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/otp-mfa.dir/screenshot_tool.cpp.o -MF CMakeFiles/otp-mfa.dir/screenshot_tool.cpp.o.d -o CMakeFiles/otp-mfa.dir/screenshot_tool.cpp.o -c /home/oorlovsk/Downloads/otp-mfa-fisrt/screenshot_tool.cpp

CMakeFiles/otp-mfa.dir/screenshot_tool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/otp-mfa.dir/screenshot_tool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oorlovsk/Downloads/otp-mfa-fisrt/screenshot_tool.cpp > CMakeFiles/otp-mfa.dir/screenshot_tool.cpp.i

CMakeFiles/otp-mfa.dir/screenshot_tool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/otp-mfa.dir/screenshot_tool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oorlovsk/Downloads/otp-mfa-fisrt/screenshot_tool.cpp -o CMakeFiles/otp-mfa.dir/screenshot_tool.cpp.s

# Object files for target otp-mfa
otp__mfa_OBJECTS = \
"CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/otp-mfa.dir/main.cpp.o" \
"CMakeFiles/otp-mfa.dir/otp_window.cpp.o" \
"CMakeFiles/otp-mfa.dir/otp_generator.cpp.o" \
"CMakeFiles/otp-mfa.dir/account_manager.cpp.o" \
"CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.o" \
"CMakeFiles/otp-mfa.dir/qr_generator.cpp.o" \
"CMakeFiles/otp-mfa.dir/encryption_utils.cpp.o" \
"CMakeFiles/otp-mfa.dir/master_password_dialog.cpp.o" \
"CMakeFiles/otp-mfa.dir/settings_dialog.cpp.o" \
"CMakeFiles/otp-mfa.dir/home/oorlovsk/otp-mfa/include/qrcodegen.cpp.o" \
"CMakeFiles/otp-mfa.dir/screenshot_tool.cpp.o"

# External object files for target otp-mfa
otp__mfa_EXTERNAL_OBJECTS =

otp-mfa: CMakeFiles/otp-mfa.dir/otp-mfa_autogen/mocs_compilation.cpp.o
otp-mfa: CMakeFiles/otp-mfa.dir/main.cpp.o
otp-mfa: CMakeFiles/otp-mfa.dir/otp_window.cpp.o
otp-mfa: CMakeFiles/otp-mfa.dir/otp_generator.cpp.o
otp-mfa: CMakeFiles/otp-mfa.dir/account_manager.cpp.o
otp-mfa: CMakeFiles/otp-mfa.dir/add_account_dialog.cpp.o
otp-mfa: CMakeFiles/otp-mfa.dir/qr_generator.cpp.o
otp-mfa: CMakeFiles/otp-mfa.dir/encryption_utils.cpp.o
otp-mfa: CMakeFiles/otp-mfa.dir/master_password_dialog.cpp.o
otp-mfa: CMakeFiles/otp-mfa.dir/settings_dialog.cpp.o
otp-mfa: CMakeFiles/otp-mfa.dir/home/oorlovsk/otp-mfa/include/qrcodegen.cpp.o
otp-mfa: CMakeFiles/otp-mfa.dir/screenshot_tool.cpp.o
otp-mfa: CMakeFiles/otp-mfa.dir/build.make
otp-mfa: /usr/lib/x86_64-linux-gnu/libQt5Multimedia.so.5.15.13
otp-mfa: /usr/lib/x86_64-linux-gnu/libQt5Sql.so.5.15.13
otp-mfa: /usr/lib/x86_64-linux-gnu/libQt5Svg.so.5.15.13
otp-mfa: /usr/lib/x86_64-linux-gnu/libQt5Network.so.5.15.13
otp-mfa: /usr/local/lib/libZXing.so.2.3.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libssl.so
otp-mfa: /usr/lib/x86_64-linux-gnu/libcrypto.so
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_stitching.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_alphamat.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_aruco.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_barcode.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_bgsegm.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_bioinspired.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_ccalib.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_cvv.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_dnn_objdetect.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_dnn_superres.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_dpm.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_face.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_freetype.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_fuzzy.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_hdf.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_hfs.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_img_hash.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_intensity_transform.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_line_descriptor.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_mcc.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_quality.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_rapid.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_reg.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_rgbd.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_saliency.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_shape.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_stereo.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_structured_light.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_superres.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_surface_matching.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_tracking.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_videostab.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_viz.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_wechat_qrcode.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_xobjdetect.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_xphoto.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5.15.13
otp-mfa: /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5.15.13
otp-mfa: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5.15.13
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_datasets.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_plot.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_text.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_phase_unwrapping.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_optflow.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_ximgproc.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_video.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_videoio.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_imgcodecs.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_dnn.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.4.6.0
otp-mfa: /usr/lib/x86_64-linux-gnu/libopencv_core.so.4.6.0
otp-mfa: CMakeFiles/otp-mfa.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/oorlovsk/Downloads/otp-mfa-fisrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Linking CXX executable otp-mfa"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/otp-mfa.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/otp-mfa.dir/build: otp-mfa
.PHONY : CMakeFiles/otp-mfa.dir/build

CMakeFiles/otp-mfa.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/otp-mfa.dir/cmake_clean.cmake
.PHONY : CMakeFiles/otp-mfa.dir/clean

CMakeFiles/otp-mfa.dir/depend: otp-mfa_autogen/timestamp
	cd /home/oorlovsk/Downloads/otp-mfa-fisrt/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/oorlovsk/Downloads/otp-mfa-fisrt /home/oorlovsk/Downloads/otp-mfa-fisrt /home/oorlovsk/Downloads/otp-mfa-fisrt/build /home/oorlovsk/Downloads/otp-mfa-fisrt/build /home/oorlovsk/Downloads/otp-mfa-fisrt/build/CMakeFiles/otp-mfa.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/otp-mfa.dir/depend

