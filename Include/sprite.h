// Header file for sprite management in the C-TurtleGraphics project.
//
// This file declares the data structures and function prototypes used to manage the sprite's
// properties and behavior, such as its position, orientation, drawing color, and pen state.
// The sprite serves as the "turtle" in the graphics environment and is manipulated through
// user input to draw lines, move, and rotate.
//
// Key structures and functions:
//    - Sprite: A struct that holds the sprite's position (x, y), orientation (angle), pen state,
//      and color (RGB components).
//    - update_location: Updates the sprite's position based on its movement speed and direction.
//    - update_orientation: Adjusts the sprite's orientation based on the specified turn direction.
//    - update_pen: Modifies the sprite's pen state, determining if it draws while moving.
//    - change_color: Changes the sprite's pen color based on a user-defined color option.

#ifndef SPRITE_H
#define SPRITE_H

#include <stdbool.h>
#include <GL/gl.h>

// Enum representing possible turn directions (not used in continuous movement but kept for compatibility)
typedef enum {
    TURN_LEFT = 0,
    TURN_RIGHT = 1
} TurnDirection;

// Struct representing the sprite's properties
typedef struct {
    float x;                  // Current x-coordinate of the sprite
    float y;                  // Current y-coordinate of the sprite
    float angle;              // Current angle (orientation) of the sprite
    TurnDirection turn;       // Last turn direction
    bool pen;                 // Whether the pen is down or up
    GLfloat r, g, b;          // Current drawing color (RGB components)
} Sprite;

// Externally accessible sprite variable
extern Sprite sprite;

// Function prototypes
void update_location(float deltaTime, int windowWidth, int windowHeight);
void update_orientation(TurnDirection direction);
void update_pen(bool pen_state);
void change_color(int color_option);

#endif // SPRITE_H
