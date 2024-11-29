// Header file for utility functions used in the C-TurtleGraphics project.
//
// This file declares utility functions that provide helper functionality across the project.
// Specifically, it includes error-checking utilities related to OpenGL calls.
//
// Key functions:
//    - checkOpenGLError: Checks for OpenGL errors after an OpenGL call and prints an error message
//      if any issues are detected. This helps in debugging OpenGL operations by ensuring that the
//      rendering process does not encounter errors silently.

#ifndef UTILITIES_H
#define UTILITIES_H

#include <GL/gl.h>

// Function prototypes
void checkOpenGLError(const char* stmt);

#endif // UTILITIES_H
