/*
 * graphics.h - Header file for the graphics rendering functions in C-TurtleGraphics.
 *
 * This header file contains the declarations for the functions responsible for handling
 * OpenGL rendering operations, including setting up the OpenGL context, loading textures,
 * rendering 2D shapes (such as lines and sprites), and displaying text on the screen.
 * It also includes necessary OpenGL and SDL2 headers for window management, image loading,
 * and text rendering. The functions declared in this file are implemented in `graphics.c` and
 * are used throughout the C-TurtleGraphics program to manage graphical output and user interaction.
 *
 * Key Functions:
 *    - setup_opengl: Initializes OpenGL for 2D rendering, including setting up the viewport
 *      and projection matrix.
 *    - load_sprite: Loads an image as a texture and prepares it for use as the turtle sprite.
 *    - render_scene: Renders the entire scene, including lines, the sprite, and text.
 *    - render_text: Renders text on the screen by creating a texture from the provided text.
 *
 * Libraries:
 *    - SDL2 for window management and text rendering.
 *    - OpenGL for rendering 2D graphics.
 *    - SDL_ttf for loading and rendering text as textures.
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL_ttf.h>

#include "sprite.h"
#include "utilities.h"

// Function prototypes
void setup_opengl(int windowWidth, int windowHeight);
bool load_sprite(const char* bmp);
void render_scene(int windowWidth, int windowHeight);
GLuint render_text(const char* text, SDL_Color color, int* w, int* h);

#endif // GRAPHICS_H
