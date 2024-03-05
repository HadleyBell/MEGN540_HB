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
CMAKE_SOURCE_DIR = /home/hadley/Documents/MEGN540_HB

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hadley/Documents/MEGN540_HB

# Utility rule file for upload_Lab5.

# Include the progress variables for this target.
include Lab5-Control/CMakeFiles/upload_Lab5.dir/progress.make

Lab5-Control/CMakeFiles/upload_Lab5: Lab5-Control/Lab5-atmega32u4.hex
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/hadley/Documents/MEGN540_HB/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Uploading Lab5-atmega32u4.hex to atmega32u4 using avr109"
	cd /home/hadley/Documents/MEGN540_HB/Lab5-Control && avrdude -p atmega32u4 -c avr109 -U flash:w:Lab5-atmega32u4.hex -P /dev/ttyZumoCarAVR

Lab5-Control/Lab5-atmega32u4.hex: Lab5-Control/Lab5-atmega32u4.elf
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/hadley/Documents/MEGN540_HB/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating Lab5-atmega32u4.hex"
	cd /home/hadley/Documents/MEGN540_HB/Lab5-Control && /usr/bin/avr-objcopy -j .text -j .data -O ihex Lab5-atmega32u4.elf Lab5-atmega32u4.hex
	cd /home/hadley/Documents/MEGN540_HB/Lab5-Control && /usr/bin/avr-size -C --mcu=atmega32u4 Lab5-atmega32u4.elf

upload_Lab5: Lab5-Control/CMakeFiles/upload_Lab5
upload_Lab5: Lab5-Control/Lab5-atmega32u4.hex
upload_Lab5: Lab5-Control/CMakeFiles/upload_Lab5.dir/build.make

.PHONY : upload_Lab5

# Rule to build all files generated by this target.
Lab5-Control/CMakeFiles/upload_Lab5.dir/build: upload_Lab5

.PHONY : Lab5-Control/CMakeFiles/upload_Lab5.dir/build

Lab5-Control/CMakeFiles/upload_Lab5.dir/clean:
	cd /home/hadley/Documents/MEGN540_HB/Lab5-Control && $(CMAKE_COMMAND) -P CMakeFiles/upload_Lab5.dir/cmake_clean.cmake
.PHONY : Lab5-Control/CMakeFiles/upload_Lab5.dir/clean

Lab5-Control/CMakeFiles/upload_Lab5.dir/depend:
	cd /home/hadley/Documents/MEGN540_HB && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hadley/Documents/MEGN540_HB /home/hadley/Documents/MEGN540_HB/Lab5-Control /home/hadley/Documents/MEGN540_HB /home/hadley/Documents/MEGN540_HB/Lab5-Control /home/hadley/Documents/MEGN540_HB/Lab5-Control/CMakeFiles/upload_Lab5.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Lab5-Control/CMakeFiles/upload_Lab5.dir/depend

