#!/bin/bash

################################################################################
# build_and_run.sh
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

set -e

echo "BUILD and RUN"

echo "BUILDING..."
./build.sh

# Get the main folder name (project root)
BUILD_DIR=$(basename "$PWD")

cd build

# Ensure binary exists
if [ ! -f ./"$BUILD_DIR" ]; then
    echo "Error: Binary not found!"
    exit 1
fi

echo "RUNNING..."
./"$BUILD_DIR"