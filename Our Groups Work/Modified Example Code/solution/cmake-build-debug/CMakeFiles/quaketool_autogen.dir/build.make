# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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
CMAKE_COMMAND = /uolstore/home/student_lnxhome01/ll22jls/Software/clion-2024.2.2/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /uolstore/home/student_lnxhome01/ll22jls/Software/clion-2024.2.2/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /uolstore/home/users/ll22jls/UI-COMP2811/cwk2_solution_final/solution

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /uolstore/home/users/ll22jls/UI-COMP2811/cwk2_solution_final/solution/cmake-build-debug

# Utility rule file for quaketool_autogen.

# Include any custom commands dependencies for this target.
include CMakeFiles/quaketool_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/quaketool_autogen.dir/progress.make

CMakeFiles/quaketool_autogen: quaketool_autogen/timestamp

quaketool_autogen/timestamp: /usr/lib64/qt6/libexec/moc
quaketool_autogen/timestamp: /usr/lib64/qt6/libexec/uic
quaketool_autogen/timestamp: CMakeFiles/quaketool_autogen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/uolstore/home/users/ll22jls/UI-COMP2811/cwk2_solution_final/solution/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target quaketool"
	/uolstore/home/student_lnxhome01/ll22jls/Software/clion-2024.2.2/bin/cmake/linux/x64/bin/cmake -E cmake_autogen /uolstore/home/users/ll22jls/UI-COMP2811/cwk2_solution_final/solution/cmake-build-debug/CMakeFiles/quaketool_autogen.dir/AutogenInfo.json Debug
	/uolstore/home/student_lnxhome01/ll22jls/Software/clion-2024.2.2/bin/cmake/linux/x64/bin/cmake -E touch /uolstore/home/users/ll22jls/UI-COMP2811/cwk2_solution_final/solution/cmake-build-debug/quaketool_autogen/timestamp

quaketool_autogen: CMakeFiles/quaketool_autogen
quaketool_autogen: quaketool_autogen/timestamp
quaketool_autogen: CMakeFiles/quaketool_autogen.dir/build.make
.PHONY : quaketool_autogen

# Rule to build all files generated by this target.
CMakeFiles/quaketool_autogen.dir/build: quaketool_autogen
.PHONY : CMakeFiles/quaketool_autogen.dir/build

CMakeFiles/quaketool_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/quaketool_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/quaketool_autogen.dir/clean

CMakeFiles/quaketool_autogen.dir/depend:
	cd /uolstore/home/users/ll22jls/UI-COMP2811/cwk2_solution_final/solution/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /uolstore/home/users/ll22jls/UI-COMP2811/cwk2_solution_final/solution /uolstore/home/users/ll22jls/UI-COMP2811/cwk2_solution_final/solution /uolstore/home/users/ll22jls/UI-COMP2811/cwk2_solution_final/solution/cmake-build-debug /uolstore/home/users/ll22jls/UI-COMP2811/cwk2_solution_final/solution/cmake-build-debug /uolstore/home/users/ll22jls/UI-COMP2811/cwk2_solution_final/solution/cmake-build-debug/CMakeFiles/quaketool_autogen.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/quaketool_autogen.dir/depend
