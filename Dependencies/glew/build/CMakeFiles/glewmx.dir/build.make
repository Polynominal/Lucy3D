# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.4

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files (x86)\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files (x86)\CMake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Lucia\Dependencies\glew\build\cmake

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Lucia\Dependencies\glew\build

# Include any dependencies generated for this target.
include CMakeFiles/glewmx.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/glewmx.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/glewmx.dir/flags.make

CMakeFiles/glewmx.dir/D_/Lucia/Dependencies/glew/src/glew.obj: CMakeFiles/glewmx.dir/flags.make
CMakeFiles/glewmx.dir/D_/Lucia/Dependencies/glew/src/glew.obj: CMakeFiles/glewmx.dir/includes_C.rsp
CMakeFiles/glewmx.dir/D_/Lucia/Dependencies/glew/src/glew.obj: D:/Lucia/Dependencies/glew/src/glew.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Lucia\Dependencies\glew\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/glewmx.dir/D_/Lucia/Dependencies/glew/src/glew.obj"
	C:\mingw64\bin\gcc.exe  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\glewmx.dir\D_\Lucia\Dependencies\glew\src\glew.obj   -c D:\Lucia\Dependencies\glew\src\glew.c

CMakeFiles/glewmx.dir/D_/Lucia/Dependencies/glew/src/glew.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glewmx.dir/D_/Lucia/Dependencies/glew/src/glew.i"
	C:\mingw64\bin\gcc.exe  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\Lucia\Dependencies\glew\src\glew.c > CMakeFiles\glewmx.dir\D_\Lucia\Dependencies\glew\src\glew.i

CMakeFiles/glewmx.dir/D_/Lucia/Dependencies/glew/src/glew.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glewmx.dir/D_/Lucia/Dependencies/glew/src/glew.s"
	C:\mingw64\bin\gcc.exe  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\Lucia\Dependencies\glew\src\glew.c -o CMakeFiles\glewmx.dir\D_\Lucia\Dependencies\glew\src\glew.s

CMakeFiles/glewmx.dir/D_/Lucia/Dependencies/glew/src/glew.obj.requires:

.PHONY : CMakeFiles/glewmx.dir/D_/Lucia/Dependencies/glew/src/glew.obj.requires

CMakeFiles/glewmx.dir/D_/Lucia/Dependencies/glew/src/glew.obj.provides: CMakeFiles/glewmx.dir/D_/Lucia/Dependencies/glew/src/glew.obj.requires
	$(MAKE) -f CMakeFiles\glewmx.dir\build.make CMakeFiles/glewmx.dir/D_/Lucia/Dependencies/glew/src/glew.obj.provides.build
.PHONY : CMakeFiles/glewmx.dir/D_/Lucia/Dependencies/glew/src/glew.obj.provides

CMakeFiles/glewmx.dir/D_/Lucia/Dependencies/glew/src/glew.obj.provides.build: CMakeFiles/glewmx.dir/D_/Lucia/Dependencies/glew/src/glew.obj


# Object files for target glewmx
glewmx_OBJECTS = \
"CMakeFiles/glewmx.dir/D_/Lucia/Dependencies/glew/src/glew.obj"

# External object files for target glewmx
glewmx_EXTERNAL_OBJECTS =

bin/glew32mx.dll: CMakeFiles/glewmx.dir/D_/Lucia/Dependencies/glew/src/glew.obj
bin/glew32mx.dll: CMakeFiles/glewmx.dir/build.make
bin/glew32mx.dll: CMakeFiles/glewmx.dir/linklibs.rsp
bin/glew32mx.dll: CMakeFiles/glewmx.dir/objects1.rsp
bin/glew32mx.dll: CMakeFiles/glewmx.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Lucia\Dependencies\glew\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library bin\glew32mx.dll"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\glewmx.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/glewmx.dir/build: bin/glew32mx.dll

.PHONY : CMakeFiles/glewmx.dir/build

CMakeFiles/glewmx.dir/requires: CMakeFiles/glewmx.dir/D_/Lucia/Dependencies/glew/src/glew.obj.requires

.PHONY : CMakeFiles/glewmx.dir/requires

CMakeFiles/glewmx.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\glewmx.dir\cmake_clean.cmake
.PHONY : CMakeFiles/glewmx.dir/clean

CMakeFiles/glewmx.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Lucia\Dependencies\glew\build\cmake D:\Lucia\Dependencies\glew\build\cmake D:\Lucia\Dependencies\glew\build D:\Lucia\Dependencies\glew\build D:\Lucia\Dependencies\glew\build\CMakeFiles\glewmx.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/glewmx.dir/depend
