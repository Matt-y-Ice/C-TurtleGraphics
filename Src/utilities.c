//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~LICENSE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
 * Copyright (C) 2024 Matthew A. Wilkerson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
 * utilities.c - Utility functions for OpenGL error checking.
 *
 * This file contains helper functions for managing OpenGL-related tasks.
 * The primary function in this file, `checkOpenGLError`, is used to check for
 * OpenGL errors after a specific OpenGL statement is executed. If an error is detected,
 * it prints the error code along with the statement that caused the error, aiding in
 * debugging and ensuring that OpenGL operations are executed successfully.
 *
 * The `checkOpenGLError` function is useful for ensuring that OpenGL operations
 * are completed without issues and provides valuable debugging information when
 * an error occurs.
 */

//==================== Header Files ====================
#include "utilities.h"
#include <stdio.h>
#include <GL/glu.h>


//==================== Function Definition ====================
void checkOpenGLError(const char* stmt) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("OpenGL error %08x, at %s\n", err, stmt);
    }
}
