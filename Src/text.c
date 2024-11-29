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
 * text.c - Functions for text rendering in OpenGL using SDL_ttf.
 *
 * This source file contains functions for initializing, rendering, and cleaning up text in an OpenGL context
 * using the SDL_ttf library. The text is rendered by first creating a surface from the provided font and text,
 * converting it into an OpenGL texture, and then rendering it as a textured quad.
 *
 * The key functions in this file include:
 *    - `init_font`: Loads and initializes the font for text rendering.
 *    - `close_font`: Frees the font resources.
 *    - `render_text`: Renders text to an OpenGL texture and returns its texture ID, as well as its width and height.
 *
 * This file handles all aspects of rendering 2D text to the screen using OpenGL and ensures proper
 * initialization, error handling, and cleanup for text resources.
 */


//==================== Header Files ====================
#include "text.h"
#include "utilities.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <GL/gl.h>
#include <GL/glu.h>  
#include <stdbool.h>  
#include <stdio.h>


//==================== Global Variables ====================
static TTF_Font* font = NULL;


//==================== Function Definitions ====================
bool init_font(const char* fontPath, const int fontSize) {
/*
 * init_font - Initializes the font for text rendering.
 *
 * This function attempts to load a TrueType font from the given file path and sets the font size
 * based on the specified `fontSize`. If the font is loaded successfully, it returns true,
 * otherwise it prints an error message and returns false.
 *
 * Parameters:
 *    fontPath   - A string representing the file path to the TrueType font file.
 *    fontSize   - The size of the font to be loaded.
 *
 * Returns:
 *    true if the font was successfully loaded, false otherwise.
 */

    font = TTF_OpenFont(fontPath, fontSize);
    if (!font) {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        return false;
    }
    return true; 
}


void close_font() {
/*
 * close_font - Closes the currently loaded font and frees associated resources.
 *
 * This function checks if a font has been loaded, and if so, it closes the font using
 * `TTF_CloseFont` and sets the font pointer to NULL. This ensures that the font is properly
 * unloaded and memory is freed, preventing memory leaks.
 *
 * Parameters:
 *    None
 *
 * Returns:
 *    None
 */

    if (font) {
        TTF_CloseFont(font);
        font = NULL;
    }
}


GLuint render_text(const char* text, const SDL_Color color, int* w, int* h) {
/*
 * render_text - Renders text to an OpenGL texture.
 *
 * This function takes a string of text and renders it to an SDL surface using the specified font and color.
 * The surface is then converted to a format suitable for OpenGL, and the resulting data is uploaded as an OpenGL texture.
 * It returns the OpenGL texture ID associated with the rendered text. The width and height of the rendered text
 * are returned via the `w` and `h` pointers.
 *
 * Parameters:
 *    text    - The text string to be rendered.
 *    color   - The SDL_Color structure specifying the color of the text.
 *    w       - Pointer to an integer where the width of the rendered text will be stored.
 *    h       - Pointer to an integer where the height of the rendered text will be stored.
 *
 * Returns:
 *    GLuint - The OpenGL texture ID associated with the rendered text, or 0 if there was an error.
 */

    // Render text to a surface
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, color);
    if (!textSurface) {
        printf("Unable to render text surface! TTF_Error: %s\n", TTF_GetError());
        return 0;
    }

    // Convert the surface to a format suitable for OpenGL texture
    SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(textSurface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(textSurface);

    if (!formattedSurface) {
        printf("Error converting text surface: %s\n", SDL_GetError());
        return 0;
    }

    // Generate a texture ID
    GLuint textureID;
    glGenTextures(1, &textureID);

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set pixel storage mode
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Set alignment to 1 for tight packing

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, formattedSurface->w, formattedSurface->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, formattedSurface->pixels);

    // Check for OpenGL errors
    const GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("OpenGL error after glTexImage2D: %p\n", gluErrorString(err));
    }

    // Set the width and height
    *w = formattedSurface->w;
    *h = formattedSurface->h;

    // Free the surface
    SDL_FreeSurface(formattedSurface);

    return textureID;
}
