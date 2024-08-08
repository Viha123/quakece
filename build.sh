#!/bin/bash

# Ensure the build directory is clean
rm -rf build && mkdir build

# Fetch SFML dependency and configure the project with CMake
cmake -S . -B build

# Build the project
cmake --build build