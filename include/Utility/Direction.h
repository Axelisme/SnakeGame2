#ifndef DIRECTION_H
#define DIRECTION_H

typedef enum Direction {
    DIRECTION_UP    = 0,
    DIRECTION_DOWN  = 1,
    DIRECTION_LEFT  = 2,
    DIRECTION_RIGHT = 3,
    DIRECTION_NONE
} Direction;

float Direction_to_angle(Direction direction);
Direction Opposite_direction(Direction direction);

#endif // DIRECTION_H