

################################################################################
# CMakeList.txt
#
# Copyright (C) 2026 Styxie-x (https://github.com/Styxie-x)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 3 (GPL v3)
# as published by the Free Software Foundation, either version 3 of the License,
# or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.
################################################################################

cmake_minimum_required(VERSION 3.10)

# Automatically set the project name to the directory name
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Set the project name and version
project(${PROJECT_NAME} VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(NCURSES REQUIRED ncurses)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/docs)
include_directories(${CMAKE_SOURCE_DIR}/exemples)
include_directories(${CMAKE_SOURCE_DIR}/external)
include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(lib/app)
include_directories(lib/button) # Si tu as un dossier button séparé


# Include directories for header files
include_directories("${CMAKE_SOURCE_DIR}/lib")


# Find all .cpp files in the src and lib directories (including subdirectories)
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/lib/*/*.cpp"
)

# Find all header files in the lib directory (including subdirectories)
file(GLOB_RECURSE HEADERS
    "${CMAKE_SOURCE_DIR}/lib/*/*.h"
)



# Add executable target, passing in all the .cpp files found
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link ncurses library
target_link_libraries(${PROJECT_NAME} ${NCURSES_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${NCURSES_INCLUDE_DIRS})
