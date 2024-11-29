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
 * graphics.c - Handles the OpenGL rendering logic for the C-TurtleGraphics program.
 *
 * This file is responsible for setting up and managing OpenGL rendering contexts,
 * loading and rendering sprites, drawing lines, and displaying real-time status information.
 * It includes the necessary setup for 2D rendering with OpenGL, using orthographic projection
 * and transformations for sprite rotation, as well as handling dynamic line drawing based on
 * user input. Additionally, text rendering and updating the graphical display for the turtle's
 * movement and actions are managed in this file.
 *
 * Key functions:
 *    - setup_opengl: Initializes OpenGL settings, including projection matrix and blending.
 *    - load_sprite: Loads an image as a sprite and prepares it for rendering.
 *    - render_scene: Clears the screen and renders lines, sprites, and status text.
 *    - add_line: Adds a new line to the array, dynamically reallocating memory when necessary.
 *
 * Libraries Used:
 *    - SDL2 for window management and image loading.
 *    - OpenGL for rendering 2D graphics, transformations, and texturing.
 */


//==================== Header Files ====================
#include "graphics.h"
#include "sprite.h"
#include "text.h"
#include "utilities.h"

#include <SDL2/SDL_image.h>
#include <stdio.h>


//==================== Macros ====================
#define IMG_W 50.0f
#define IMG_H 50.0f


//==================== Structure ====================
typedef struct {  // Line array structure
    GLfloat x1, y1, x2, y2;   // Coordinates of the line
    GLfloat r, g, b;          // Color of the line (RGB components)
} Line;


//==================== Global Variables ====================
GLuint spriteTextureID = 0; // Texture ID

Line* lines = NULL;
int lineCapacity = 50;
int lineCount = 0;


//==================== Function Definitions ====================
void setup_opengl(int const windowWidth, int const windowHeight) {
/*
 * setup_opengl - Initializes OpenGL settings for 2D rendering.
 *
 * This function configures the OpenGL context for rendering 2D graphics within a window,
 * including setting up the viewport, projection matrix, and model-view matrix.
 * It also initializes OpenGL blending for transparency effects and allocates memory
 * for the array of lines to be drawn by the turtle. The function prepares OpenGL
 * for rendering operations such as drawing sprites and lines with proper transformations
 * and blending.
 *
 * Parameters:
 *    windowWidth   - The width of the window to set the viewport and projection matrix.
 *    windowHeight  - The height of the window to set the viewport and projection matrix.
 */

    // Set the viewport
    glViewport(0, 0, windowWidth, windowHeight);

    // Set up the projection matrix for orthographic 2D rendering
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, windowWidth, windowHeight, 0.0);

    // Set up the model-view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set the clear color to white background
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Allocate memory for lines array
    lines = malloc(lineCapacity * sizeof(Line));
    if (!lines) {
        printf("Error allocating memory for lines!\n");
        exit(1);
    }
}


bool load_sprite(const char* bmp) {
/*
 * load_sprite - Loads an image and converts it into an OpenGL texture.
 *
 * This function loads an image file (BMP format) using SDL_image, converts it into
 * a format suitable for OpenGL, and uploads it as a texture. The texture is then
 * used to render the sprite. The function performs error checking during each step
 * to ensure the image is loaded and converted properly. It also configures OpenGL texture
 * parameters, such as wrapping and filtering modes, before uploading the texture data.
 *
 * Parameters:
 *    bmp - The file path to the BMP image to be loaded and used as a sprite texture.
 *
 * Returns:
 *    true if the image was loaded and converted successfully, false otherwise.
 */

    // Load the image using SDL_image
    SDL_Surface* tempSurface = IMG_Load(bmp);
    if (!tempSurface) {
        printf("Error loading image %s: %s\n", bmp, IMG_GetError());
        return false;
    }

    // Convert the surface to a format suitable for OpenGL texture
    SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(tempSurface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(tempSurface);

    if (!formattedSurface) {
        printf("Error converting surface: %s\n", SDL_GetError());
        return false;
    }

    // Generate a texture ID and bind it
    glGenTextures(1, &spriteTextureID);
    glBindTexture(GL_TEXTURE_2D, spriteTextureID);

    // Set pixel storage mode
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Set texture parameters after binding the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload the texture data to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, formattedSurface->w, formattedSurface->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, formattedSurface->pixels);
    checkOpenGLError("glTexImage2D");

    // Free the surface as we no longer need it
    SDL_FreeSurface(formattedSurface);

    return true;
}


void render_scene(int const windowWidth, int const windowHeight) {
/*
 * render_scene - Clears the screen and renders all graphical elements, including lines, the sprite, and text.
 *
 * This function is responsible for rendering the entire scene in the OpenGL window. It first clears the screen,
 * then renders all previously drawn lines, followed by rendering the sprite and displaying text. The function
 * also manages the OpenGL state, including enabling/disabling features such as texturing and blending, and applies
 * necessary transformations (translation, rotation) to the sprite. The text is rendered as textured quads, with the
 * sprite being drawn in a 2D orthographic projection system. Real-time status information, such as the turtle's
 * position, angle, pen state, and line color, is displayed on the screen.
 *
 * Parameters:
 *    windowWidth - The width of the window in pixels.
 *    windowHeight - The height of the window in pixels.
 */

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
    checkOpenGLError("glClear");

    // Reset OpenGL state before rendering
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // **Line Rendering**

    // Disable texturing for line rendering
    glDisable(GL_TEXTURE_2D);

    // Draw all previously drawn lines
    if (lineCount > 0) {
        glBegin(GL_LINES);
        for (int i = 0; i < lineCount; i++) {
            glColor3f(lines[i].r, lines[i].g, lines[i].b);  // Set line color
            glVertex2f(lines[i].x1, lines[i].y1);           // Start point
            glVertex2f(lines[i].x2, lines[i].y2);           // End point
        }
        glEnd();
        checkOpenGLError("Drawing lines");
    }

    // **Sprite Rendering**

    // Enable texturing for sprite rendering
    glEnable(GL_TEXTURE_2D);

    // Reset color to white to avoid tinting the sprite
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    // Bind the texture before drawing
    glBindTexture(GL_TEXTURE_2D, spriteTextureID);

    // Calculate sprite corners
    const float halfWidth = IMG_W / 2.0f;
    const float halfHeight = IMG_H / 2.0f;

    const float x = sprite.x;
    const float y = sprite.y;

    // Set up vertices
    const GLfloat vertices[] = {
        x - halfWidth, y - halfHeight,
        x + halfWidth, y - halfHeight,
        x + halfWidth, y + halfHeight,
        x - halfWidth, y + halfHeight
    };

    // Standard texture coordinates without flipping
    const GLfloat texCoords[] = {
        0.0f, 0.0f,  // Bottom-left
        1.0f, 0.0f,  // Bottom-right
        1.0f, 1.0f,  // Top-right
        0.0f, 1.0f   // Top-left
    };

    // Apply rotation around the sprite's center
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(-sprite.angle, 0, 0, 1);  // Negative because OpenGL rotates counterclockwise
    glTranslatef(-x, -y, 0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

    glDrawArrays(GL_QUADS, 0, 4);
    checkOpenGLError("glDrawArrays for sprite");

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();

    // **Text Rendering**

    // Construct the status string
    char statusText[256];
    sprintf(
        statusText,
        "Position: (%.1f, %.1f)\nAngle: %.1f degrees\nPen: %s\nLine Color: %s",
        sprite.x, sprite.y, sprite.angle,
        sprite.pen ? "Down" : "Up",
        (sprite.r == 0.0f && sprite.g == 0.0f && sprite.b == 0.0f) ? "Black" :
        (sprite.r == 0.0f && sprite.g == 0.0f && sprite.b == 1.0f) ? "Blue" :
        (sprite.r == 1.0f && sprite.g == 0.0f && sprite.b == 0.0f) ? "Red" :
        (sprite.r == 0.0f && sprite.g == 1.0f && sprite.b == 0.0f) ? "Green" :
        (sprite.r == 1.0f && sprite.g == 1.0f && sprite.b == 0.0f) ? "Yellow" :
        "Custom"
    );

    // Split the statusText into lines
    char* linesText[10];
    int lineCountText = 0;

    char* token = strtok(statusText, "\n");
    while (token != NULL && lineCountText < 10) {
        linesText[lineCountText++] = token;
        token = strtok(NULL, "\n");
    }

    // Set text color (black)
    const SDL_Color textColor = {0, 0, 0, 255};

    // Variables for text position
    float textY = 10.0f;

    // Set up orthographic projection for text rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, windowHeight, 0); // Top-left corner is (0,0)

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Enable blending and texturing
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    // Render each line
    for (int i = 0; i < lineCountText; i++) {
        int lineWidth = 0, lineHeight = 0;
        GLuint lineTextureID = render_text(linesText[i], textColor, &lineWidth, &lineHeight);

        if (lineTextureID != 0) {
            const float textX = 10.0f;
            // Bind the text texture
            glBindTexture(GL_TEXTURE_2D, lineTextureID);

            // Set up vertices and texture coordinates
            const GLfloat textVertices[] = {
                textX, textY + (float)lineHeight,
                textX + (float)lineWidth, textY + (float)lineHeight,
                textX + (float)lineWidth, textY,
                textX, textY
            };

            const GLfloat textTexCoords[] = {
                0.0f, 1.0f,  // Bottom-left
                1.0f, 1.0f,  // Bottom-right
                1.0f, 0.0f,  // Top-right
                0.0f, 0.0f   // Top-left
            };

            // Render the textured quad
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);

            glVertexPointer(2, GL_FLOAT, 0, textVertices);
            glTexCoordPointer(2, GL_FLOAT, 0, textTexCoords);

            glDrawArrays(GL_QUADS, 0, 4);

            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);

            // Update textY for the next line
            textY += (float)lineHeight + 2; // Add some spacing between lines

            // Delete the texture after rendering
            glDeleteTextures(1, &lineTextureID);
        }
    }

    // Restore matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}


void add_line(const float x1, const float y1, const float x2, const float y2) {
/*
 * add_line - Adds a new line to the lines array with the current color from the sprite.
 *
 * This function is responsible for adding a new line to the array of lines. The line is defined by two
 * points, (x1, y1) and (x2, y2), and is assigned the current color of the sprite. If the array is full,
 * it dynamically reallocates memory to accommodate more lines. The function increases the `lineCount` and
 * ensures that memory is properly allocated and freed to avoid memory leaks.
 *
 * Parameters:
 *    x1, y1 - The starting coordinates of the line.
 *    x2, y2 - The ending coordinates of the line.
 */

    if (lineCount >= lineCapacity) {
        lineCapacity *= 2;

        // Use a temporary pointer to hold the result to prevent memory leaks if failure
        Line* temp = realloc(lines, lineCapacity * sizeof(Line));

        if (!temp) {
            printf("Error reallocating memory for lines!\n");
            free(lines);  // Free the original buffer if it fails
            exit(1);
        }

        // Assign the new buffer to lines only if it succeeded
        lines = temp;
    }

    // Set line coordinates
    lines[lineCount].x1 = x1;
    lines[lineCount].y1 = y1;
    lines[lineCount].x2 = x2;
    lines[lineCount].y2 = y2;

    // Set line color from current sprite color
    lines[lineCount].r = sprite.r;
    lines[lineCount].g = sprite.g;
    lines[lineCount].b = sprite.b;

    lineCount++;
}
