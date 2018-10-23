cmake_minimum_required(VERSION 3.6)
project(cfiles)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -O0")
set(CMAKE_BUILD_TYPE Debug)

set(CFILES_VERSION_MAJOR 0)
set(CFILES_VERSION_MINOR 1)

# Include ncurses
set(CURSES_USE_NCURSES TRUE)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Configure a header file to pass some of the CMake settings to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/cfilesConfig.h.in"
  "${PROJECT_BINARY_DIR}/cfilesConfig.h"
 )

# Add the binary tree to the search path for include files so we'll find cfilesConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# Find the source files
set(SOURCE_FILES
  src/ui.c
  src/ui_menu.c
  src/main.c
  )

add_executable(cfiles ${SOURCE_FILES})
target_link_libraries(cfiles ${CURSES_LIBRARIES})
