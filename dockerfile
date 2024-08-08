# Use an official image with CMake and a C++ compiler
FROM ubuntu:20.04

# Set environment variables to avoid user prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install required packages
RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    git \
    make \
    libsfml-dev \
    libx11-dev \
    && rm -rf /var/lib/apt/lists/*

# Create a directory for the application
WORKDIR /app

# Copy the project files into the container
COPY . .
# Ensure the build directory is clean
RUN rm -rf build && mkdir build
# Fetch SFML dependency
RUN cmake -S . -B build -DFETCHCONTENT_SOURCE_DIR_SFML=/app/SFML

# Build the project
RUN cmake --build ./build

ENTRYPOINT ["./build/bin/main"]
CMD []