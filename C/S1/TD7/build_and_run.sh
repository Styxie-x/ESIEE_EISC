#!/bin/bash
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