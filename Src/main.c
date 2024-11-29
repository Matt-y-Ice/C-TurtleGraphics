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
 * main.c - Entry point for the Interactive C-Turtle Graphics program.
 *
 * This file contains the main function that initializes the application,
 * sets up the necessary libraries, and starts the main loop of the interactive
 * graphics program. It handles the initialization of SDL2 for window management,
 * SDL_image for image loading, SDL_ttf for font rendering, and OpenGL for graphics rendering.
 *
 * The program simulates a turtle graphics system where users can interactively control
 * a sprite (the "turtle") on a 2D canvas to draw lines and shapes using the arrow keys,
 * along with dynamic changes to line color and pen state. The OpenGL context is set up
 * for 2D rendering, and SDL handles event management and window resizing.
 *
 * The main loop continuously handles user input, updates the scene, and renders the
 * turtle's movements and drawn lines. It also manages OpenGL context for smooth rendering
 * and provides real-time status information about the turtle's position and state.
 *
 * Key Features:
 * - Initialization of SDL, SDL_image, SDL_ttf, and OpenGL.
 * - Set up of a window and OpenGL context for rendering 2D graphics.
 * - Handling of sprite loading, transformation, and rendering.
 * - Dynamic management of lines drawn on the screen based on user input.
 * - Real-time display of the turtle's current state, including position and color.
 *
 * Cleanup is performed at the end of the program to release resources used by SDL,
 * SDL_image, SDL_ttf, and OpenGL.
 *
 * This program is primarily designed for educational purposes, demonstrating
 * concepts of graphics rendering, event handling, and interactive programming.
 */


//==================== Header Files ====================
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

#include "graphics.h"
#include "events.h"
#include "sprite.h"
#include "text.h"


//==================== Macros ====================
#define WINDOW_TITLE "Interactive C-Turtle Graphics"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800


//==================== Global Variables ====================
SDL_Window* window = NULL;            // SDL window pointer
SDL_GLContext glContext;              // OpenGL context for rendering


//==================== Main ====================
int main(int argc, char* argv[]) {

    // Initialize window dimensions
    int windowWidth = WINDOW_WIDTH;
    int windowHeight = WINDOW_HEIGHT;

    // Initialize SDL and create window
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) > 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Request an OpenGL 2.1 context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    // Request a compatibility profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

    // Create SDL window with OpenGL support and resizable flag
    window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create OpenGL context for rendering in the window
    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        printf("Error creating OpenGL context: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize SDL_image
    int const imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! IMG_Error: %s\n", IMG_GetError());
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize font
    if (!init_font("./Fonts/DejaVuSansMNerdFont-Regular.ttf", 12)) {
        TTF_Quit();
        IMG_Quit();
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize OpenGL
    setup_opengl(windowWidth, windowHeight);

    // Load sprite
    if (!load_sprite("./Images/mateo.png")) {
        close_font();
        TTF_Quit();
        IMG_Quit();
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize sprite properties
    sprite.x = (float)windowWidth / 2.0f;
    sprite.y = (float)windowHeight / 2.0f;
    sprite.angle = 0.0f;
    sprite.pen = false;
    sprite.r = 0.0f;
    sprite.g = 0.0f;
    sprite.b = 0.0f;

    // Main loop
    bool running = true;
    while (running) {
        // Calculate deltaTime
        static Uint32 lastTime = 0;
        Uint32 const currentTime = SDL_GetTicks();
        float const deltaTime = (float)(currentTime - lastTime) / 1000.0f; // Convert milliseconds to seconds
        lastTime = currentTime;

        // Handle events
        running = handle_events(deltaTime, &windowWidth, &windowHeight);

        // Render scene
        render_scene(windowWidth, windowHeight);

        // Swap buffers to display the current frame
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    close_font();
    TTF_Quit();
    IMG_Quit();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
