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

# Utility rule file for upload_Lab1.

# Include the progress variables for this target.
include Lab1-Serial/CMakeFiles/upload_Lab1.dir/progress.make

Lab1-Serial/CMakeFiles/upload_Lab1: Lab1-Serial/Lab1-atmega32u4.hex
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/hadley/Documents/MEGN540_HB/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Uploading Lab1-atmega32u4.hex to atmega32u4 using avr109"
	cd /home/hadley/Documents/MEGN540_HB/Lab1-Serial && avrdude -p atmega32u4 -c avr109 -U flash:w:Lab1-atmega32u4.hex -P /dev/ttyZumoCarAVR

Lab1-Serial/Lab1-atmega32u4.hex: Lab1-Serial/Lab1-atmega32u4.elf
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/hadley/Documents/MEGN540_HB/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating Lab1-atmega32u4.hex"
	cd /home/hadley/Documents/MEGN540_HB/Lab1-Serial && /usr/bin/avr-objcopy -j .text -j .data -O ihex Lab1-atmega32u4.elf Lab1-atmega32u4.hex
	cd /home/hadley/Documents/MEGN540_HB/Lab1-Serial && /usr/bin/avr-size -C --mcu=atmega32u4 Lab1-atmega32u4.elf

upload_Lab1: Lab1-Serial/CMakeFiles/upload_Lab1
upload_Lab1: Lab1-Serial/Lab1-atmega32u4.hex
upload_Lab1: Lab1-Serial/CMakeFiles/upload_Lab1.dir/build.make

.PHONY : upload_Lab1

# Rule to build all files generated by this target.
Lab1-Serial/CMakeFiles/upload_Lab1.dir/build: upload_Lab1

.PHONY : Lab1-Serial/CMakeFiles/upload_Lab1.dir/build

Lab1-Serial/CMakeFiles/upload_Lab1.dir/clean:
	cd /home/hadley/Documents/MEGN540_HB/Lab1-Serial && $(CMAKE_COMMAND) -P CMakeFiles/upload_Lab1.dir/cmake_clean.cmake
.PHONY : Lab1-Serial/CMakeFiles/upload_Lab1.dir/clean

Lab1-Serial/CMakeFiles/upload_Lab1.dir/depend:
	cd /home/hadley/Documents/MEGN540_HB && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hadley/Documents/MEGN540_HB /home/hadley/Documents/MEGN540_HB/Lab1-Serial /home/hadley/Documents/MEGN540_HB /home/hadley/Documents/MEGN540_HB/Lab1-Serial /home/hadley/Documents/MEGN540_HB/Lab1-Serial/CMakeFiles/upload_Lab1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Lab1-Serial/CMakeFiles/upload_Lab1.dir/depend
