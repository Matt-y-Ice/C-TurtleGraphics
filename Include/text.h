// Header file for text rendering functions in the C-TurtleGraphics project.
//
// This file declares the function prototypes for initializing fonts, rendering text as OpenGL textures,
// and cleaning up font resources. It facilitates displaying text on the screen, such as status information,
// by rendering text with SDL_ttf and converting it into a format suitable for OpenGL rendering.
//
// Key functions:
//    - init_font: Initializes the font for text rendering from a specified file path and font size.
//    - close_font: Closes the currently loaded font and frees resources.
//    - render_text: Renders a given text string to an OpenGL texture, returning the texture ID and setting
//      the text's width and height.

#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL_ttf.h>
#include <GL/gl.h>
#include <stdbool.h>  // Add this line

// Function prototypes
bool init_font(const char* fontPath, int fontSize);
void close_font();
GLuint render_text(const char* text, SDL_Color color, int* w, int* h);

#endif // TEXT_H
