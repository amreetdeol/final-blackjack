# Stage 1: Build the C++ backend using CMake
FROM gcc:latest AS builder

WORKDIR /cpp-server

# Copy all files from the cpp-server directory to /cpp-server in the container
COPY . .

# Update package lists and install CMake
RUN apt-get update && apt-get install -y cmake

# Remove any existing build directory, then create a fresh one and build the project
RUN rm -rf build && mkdir -p build && cd build && cmake .. && make

# Stage 2: Create a minimal runtime image using Debian slim
FROM debian:stable-slim

WORKDIR /app

# Copy the compiled server binary from the builder stage into the final image
COPY --from=builder /cpp-server/build/server /app/

# Expose the server port (assumed to be 8080)
EXPOSE 8080

# Run the server binary
CMD ["./server"]
