
#include "Utility/Position.h"

Pos make_Pos(pos_t y, pos_t x) {
    Pos pos = {y, x};
    return pos;
}
bool equal(const Pos A, const Pos B) {
    return (A.y == B.y && A.x == B.x);
}
Pos add(const Pos A, const Pos B) {
    return make_Pos(A.y + B.y, A.x + B.x);
}
Pos sub(const Pos A, const Pos B) {
    return make_Pos(A.y - B.y, A.x - B.x);
}
fpos_t distance(const Pos A, const Pos B) {
    Pos del = make_Pos(A.y - B.y, A.x - B.x);
    return sqrt(del.x * del.x + del.y * del.y);
}
Pos heading_pos(const Pos pos, const DIRECTION dir) {
    return add(pos, DIR_TO_POS(dir));
}
inline Pos DIR_TO_POS(const DIRECTION dir) {
    switch(dir) {
        case DIRECTION_LEFT:  return make_Pos( 0.,-1.);
        case DIRECTION_RIGHT: return make_Pos( 0., 1.);
        case DIRECTION_UP:    return make_Pos(-1., 0.);
        case DIRECTION_DOWN:  return make_Pos( 1., 0.);
        default:             return make_Pos( 0., 0.);
    }
}
inline DIRECTION POS_TO_DIR(const Pos pos) {
    if (pos.x == 0 && pos.y == 0) return DIRECTION_NONE;
    if (fabs(pos.x) > fabs(pos.y)) {
        if (pos.x > 0) return DIRECTION_RIGHT;
        else return DIRECTION_LEFT;
    } else {
        if (pos.y > 0) return DIRECTION_DOWN;
        else return DIRECTION_UP;
    }
}