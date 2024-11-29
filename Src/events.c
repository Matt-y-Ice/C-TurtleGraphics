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
 * handle_events - Handles user input events and updates the turtle's state.
 *
 * This function processes SDL events, such as key presses, window events, and
 * window resizing. It updates the state of the turtle (sprite) based on user input,
 * including its position, rotation, and pen state. The function also handles window
 * resizing and adjusts the OpenGL viewport and projection accordingly.
 *
 * The function processes key events such as:
 *    - Arrow keys to rotate the turtle.
 *    - 'D' and 'U' to toggle the pen state (drawing on/off).
 *    - Number keys (1-5) to change the drawing color.
 *    - The Escape key to exit the program.
 *
 * Additionally, the function updates the turtle's position if the "Up" arrow key
 * is pressed, and if the pen is down, it adds a line to the drawing.
 *
 * Parameters:
 *    deltaTime     - The time elapsed since the last frame, used for smooth movement.
 *    windowWidth   - Pointer to the window's width (to be updated on window resize).
 *    windowHeight  - Pointer to the window's height (to be updated on window resize).
 *
 * Returns:
 *    true if the program should continue running, false if the program should exit.
 */


//==================== Header Files ====================
#include "events.h"
#include "sprite.h"
#include "graphics.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>


//==================== Global Variables ====================
// Key state variables for continuous movement and rotation
static bool keyLeftPressed = false;
static bool keyRightPressed = false;
static bool keyUpPressed = false;


//==================== Macros ====================
// Movement and rotation increments
#define ROTATION_INCREMENT 90.0f    // Degrees per second for rotation
#define MOVE_INCREMENT 200.0f       // Pixels per second for movement


//==================== Function Definition ====================
bool handle_events(const float deltaTime, int* windowWidth, int* windowHeight) {
    SDL_Event evt;

    // Process all pending events
    while (SDL_PollEvent(&evt) != 0) {
        switch (evt.type) {
            case SDL_QUIT:
                return false; // Exit on quit event
            case SDL_KEYDOWN:
                switch (evt.key.keysym.sym) {
                    case SDLK_RIGHT:
                        keyRightPressed = true;
                        break;
                    case SDLK_LEFT:
                        keyLeftPressed = true;
                        break;
                    case SDLK_UP:
                        keyUpPressed = true;
                        break;
                    case SDLK_d:
                        update_pen(true);
                        break;
                    case SDLK_u:
                        update_pen(false);
                        break;
                    case SDLK_1:
                        change_color(1);
                        break;
                    case SDLK_2:
                        change_color(2);
                        break;
                    case SDLK_3:
                        change_color(3);
                        break;
                    case SDLK_4:
                        change_color(4);
                        break;
                    case SDLK_5:
                        change_color(5);
                        break;
                    case SDLK_ESCAPE:
                        return false; // Exit on ESC key
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (evt.key.keysym.sym) {
                    case SDLK_RIGHT:
                        keyRightPressed = false;
                        break;
                    case SDLK_LEFT:
                        keyLeftPressed = false;
                        break;
                    case SDLK_UP:
                        keyUpPressed = false;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_WINDOWEVENT:
                if (evt.window.event == SDL_WINDOWEVENT_RESIZED) {
                    *windowWidth = evt.window.data1;
                    *windowHeight = evt.window.data2;
                    printf("Window resized to %d x %d\n", *windowWidth, *windowHeight);

                    // Update OpenGL viewport and projection
                    glViewport(0, 0, *windowWidth, *windowHeight);
                    glMatrixMode(GL_PROJECTION);
                    glLoadIdentity();
                    gluOrtho2D(0.0, *windowWidth, *windowHeight, 0.0);
                    glMatrixMode(GL_MODELVIEW);
                }
                break;
            default:
                continue;
        }
    }

    // Update rotation
    if (keyLeftPressed) {
        sprite.angle = fmodf(sprite.angle + ROTATION_INCREMENT * deltaTime, 360.0f);
    }
    if (keyRightPressed) {
        sprite.angle = fmodf(sprite.angle - ROTATION_INCREMENT * deltaTime + 360.0f, 360.0f);
    }

    // Update position
    if (keyUpPressed) {
        const float previous_x = sprite.x;
        const float previous_y = sprite.y;
        update_location(deltaTime, *windowWidth, *windowHeight);
        if (sprite.pen) {
            // Add line to lines array
            extern void add_line(float x1, float y1, float x2, float y2);
            add_line(previous_x, previous_y, sprite.x, sprite.y);
        }
    }

    return true;
}
