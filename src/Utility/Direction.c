
#undef __STRICT_ANSI__
#include "Utility/Direction.h"
#include "math.h"

float Direction_to_angle(Direction direction) {
    switch (direction) {
        case DIRECTION_UP:    return 0.0f;
        case DIRECTION_RIGHT: return M_PI_2;
        case DIRECTION_DOWN:  return M_PI;
        case DIRECTION_LEFT:  return M_PI_2 * 3;
        default:              return 0.0f;
    }
}