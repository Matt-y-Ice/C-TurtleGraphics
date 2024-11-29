// Header file for event handling in the C-TurtleGraphics project.
//
// This file declares the function prototypes for handling user input events,
// such as key presses, window resizing, and other SDL events. The event handler
// processes input to update the game state, such as moving the turtle or changing its pen state.
//
// Includes:
//    - Standard boolean type from <stdbool.h>
//
// Main function:
//    - handle_events: Processes SDL events, including key presses and window events,
//      and updates the game state based on user input.

#ifndef EVENTS_H
#define EVENTS_H

#include <stdbool.h>

// Function prototypes
bool handle_events(float deltaTime, int* windowWidth, int* windowHeight);

#endif // EVENTS_H
