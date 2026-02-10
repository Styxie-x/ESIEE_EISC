#!/bin/bash

################################################################################
# build.sh
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

# Define the build directory
BUILD_DIR="build"

# Remove the previous build directory if it exists
if [ -d "$BUILD_DIR" ]; then
    echo "Removing previous build directory..."
    rm -rf "$BUILD_DIR"
fi

# Run CMake to configure the project
echo "Configuring project with CMake..."
cmake -S . -B "$BUILD_DIR"

# Build the project
echo "Building the project..."
cmake --build "$BUILD_DIR"

echo "Build complete!"

echo "built program stored in /build"
