#!/bin/bash

# Parse command-line arguments
while [[ "$#" -gt 0 ]]; do
    case $1 in
        -d|--debug) BUILD_TYPE="Debug";;
        -r|--release) BUILD_TYPE="Release";;
        *) echo "Unknown parameter: $1"; exit 1;;
    esac
    shift
done

# Set default build type if not provided
if [ -z "$BUILD_TYPE" ]; then
    BUILD_TYPE="Release"
fi

# Run CMake with the specified build type
cmake -S . -B build/ -DCMAKE_BUILD_TYPE=$BUILD_TYPE

cmake --build build/