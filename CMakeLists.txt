# CMake configuration file for the C-TurtleGraphics project.
#
# This file sets up the build process for the C-TurtleGraphics project using CMake.
# It specifies the minimum CMake version required, defines compiler flags for different
# build configurations (debug and release), and includes necessary directories for header files.
# The source files for the project are listed, and directories for object files and binaries are created.
# It also specifies the required libraries for linking and sets the output locations for the binary and object files.
# Finally, an executable target is created and linked with the specified libraries.

# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(C-TurtleGraphics C)

# Compiler flags
set(CMAKE_C_FLAGS "-Wall -O2")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3")

# Include directories
include_directories(
        ${PROJECT_SOURCE_DIR}/Include
        /usr/include
        /usr/include/GL
)

# Source files
set(SOURCE_FILES
        Src/main.c
        Src/graphics.c
        Src/events.c
        Src/sprite.c
        Src/text.c
        Src/utilities.c
)

# Output directories
set(BIN_DIR ${PROJECT_SOURCE_DIR}/Binaries)
set(OBJ_DIR ${PROJECT_SOURCE_DIR}/Obj)

# Create output directories
file(MAKE_DIRECTORY ${BIN_DIR})
file(MAKE_DIRECTORY ${OBJ_DIR})

# Linker flags
set(LIBRARIES
        SDL2
        SDL2_image
        SDL2_ttf
        GL
        GLU
        dl
        m
)

# Add executable target
add_executable(turtle ${SOURCE_FILES})

# Set the output directory for the binary
set_target_properties(turtle PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)

# Link libraries to the executable
target_link_libraries(turtle PRIVATE ${LIBRARIES})
