# Use Ubuntu as base image for development environment
FROM ubuntu:22.04

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install all required dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    make \
    valgrind \
    gdb \
    git \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /workspace

# Copy project files
COPY . .

# Create bin directory if it doesn't exist
RUN mkdir -p bin

# Build the project
RUN make clean && make

# Set the default command
CMD ["./bin/dj_manager"]

