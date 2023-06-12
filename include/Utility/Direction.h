#ifndef DIRECTION_H
#define DIRECTION_H

typedef enum Direction {
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_NONE
} Direction;

float Direction_to_angle(Direction direction);

#endif // DIRECTION_H