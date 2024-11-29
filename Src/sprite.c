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
 * File: sprite.c
 *
 * Description:
 * This file contains the functions responsible for updating and controlling the sprite's behavior
 * in the Interactive Turtle Graphics program. The main operations involve handling the sprite's
 * movement, rotation, pen state (up or down), and color changes. The sprite represents the "turtle"
 * that draws lines based on user input. It interacts with OpenGL for rendering and responds to SDL events
 * to update its position and properties in real-time.
 *
 * Functions:
 *    - update_location: Calculates and updates the sprite's position based on its current angle and speed,
 *      while ensuring the sprite stays within the window boundaries.
 *    - update_pen: Updates the pen state of the sprite (pen up or pen down), determining whether it draws
 *      lines as it moves.
 *    - change_color: Changes the color of the turtle's pen based on user input. Different integer options
 *      correspond to different colors.
 *
 * Dependencies:
 *    - sprite.h: Contains the declaration of the Sprite structure and related functions.
 *    - math.h: For trigonometric calculations to update the sprite's movement.
 *    - stdio.h: For input/output operations, including error reporting.
 *
 * This file allows the main application to control the movement, drawing, and appearance of the sprite,
 * enabling interactive graphics where users can create drawings by controlling the turtle.
 */


//==================== Header Files ====================
#include "sprite.h"
#include <math.h>
#include <stdio.h>


//==================== Global Variables ====================
// Global sprite variable
Sprite sprite;


//==================== Macros ====================
// Movement and rotation increments
#define MOVE_INCREMENT 200.0f       // Pixels per second for movement


//==================== Function Definitions ====================
void update_location(const float deltaTime, const int windowWidth, const int windowHeight) {
/*
 * handle_events - Processes SDL events and updates the game state based on user input.
 *
 * This function is responsible for handling various SDL events, such as key presses, key releases,
 * window resizing, and other events. It updates the game state accordingly, such as moving the sprite,
 * rotating it, or changing the pen state. The function also manages window resizing by adjusting the
 * OpenGL viewport and projection. It handles both key press (SDL_KEYDOWN) and key release (SDL_KEYUP)
 * events to control the turtle’s movement and other actions. Additionally, it returns a boolean value
 * indicating whether the main game loop should continue running or exit.
 *
 * Parameters:
 *    deltaTime    - The time difference between the current and the previous frame, used to ensure
 *                   smooth and frame-independent movement.
 *    windowWidth  - Pointer to the width of the window. This is updated in case of a window resize.
 *    windowHeight - Pointer to the height of the window. This is updated in case of a window resize.
 *
 * Returns:
 *    true if the game loop should continue, false if the game loop should exit (e.g., on quitting or
 *    pressing the ESC key).
 */

    // Calculate potential new position
    const float deltaX = MOVE_INCREMENT * cos(sprite.angle * M_PI / 180.0f) * deltaTime;
    const float deltaY = -MOVE_INCREMENT * sin(sprite.angle * M_PI / 180.0f) * deltaTime;  // Negative due to coordinate system

    float newX = sprite.x + deltaX;
    float newY = sprite.y + deltaY;

    // Calculate half sprite dimensions
    const float halfWidth = 25.0f;  // IMG_W / 2.0f
    const float halfHeight = 25.0f; // IMG_H / 2.0f

    // Check boundaries on X-axis
    if (newX - halfWidth < 0) {
        newX = halfWidth;
    } else if (newX + halfWidth > (float)windowWidth) {
        newX = (float)windowWidth - halfWidth;
    }

    // Check boundaries on Y-axis
    if (newY - halfHeight < 0) {
        newY = halfHeight;
    } else if (newY + halfHeight > (float)windowHeight) {
        newY = (float)windowHeight - halfHeight;
    }

    // Update sprite position
    sprite.x = newX;
    sprite.y = newY;
}


void update_pen(const bool pen_state) {
/*
 * update_pen - Updates the state of the turtle's pen (up or down).
 *
 * This function updates the `pen` state of the sprite, which determines whether the turtle is
 * drawing lines as it moves. When the pen is down (`pen_state` is true), the turtle will draw lines,
 * and when the pen is up (`pen_state` is false), the turtle will not draw lines.
 *
 * Parameters:
 *    pen_state - A boolean value indicating the desired state of the pen.
 *                true means the pen is down (drawing), false means the pen is up (not drawing).
 */

    sprite.pen = pen_state;
}


void change_color(const int color_option) {
/*
 * change_color - Changes the color of the turtle's pen.
 *
 * This function updates the color of the sprite's pen based on the provided `color_option`.
 * Each color option corresponds to a specific RGB color, and the pen color is set accordingly.
 * If an invalid color option is provided, an error message is printed. The available colors are:
 *    1 - Black
 *    2 - Blue
 *    3 - Red
 *    4 - Green
 *    5 - Yellow
 *
 * Parameters:
 *    color_option - An integer representing the color choice. Valid options are:
 *                   1 (Black), 2 (Blue), 3 (Red), 4 (Green), and 5 (Yellow).
 */

    switch (color_option) {
        case 1: // Black
            sprite.r = 0.0f;
            sprite.g = 0.0f;
            sprite.b = 0.0f;
            printf("Color changed to Black\n");
            break;
        case 2: // Blue
            sprite.r = 0.0f;
            sprite.g = 0.0f;
            sprite.b = 1.0f;
            printf("Color changed to Blue\n");
            break;
        case 3: // Red
            sprite.r = 1.0f;
            sprite.g = 0.0f;
            sprite.b = 0.0f;
            printf("Color changed to Red\n");
            break;
        case 4: // Green
            sprite.r = 0.0f;
            sprite.g = 1.0f;
            sprite.b = 0.0f;
            printf("Color changed to Green\n");
            break;
        case 5: // Yellow
            sprite.r = 1.0f;
            sprite.g = 1.0f;
            sprite.b = 0.0f;
            printf("Color changed to Yellow\n");
            break;
        default:
            printf("Invalid color option\n");
            break;
    }
}
