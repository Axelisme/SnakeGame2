#include<Position.h>
#include<math.h>

// Pos
Pos makePos(pos_t y, pos_t x) {
    Pos pos = {y, x};
    return pos;
}
bool equal(const Pos A, const Pos B) {
    return (A.y == B.y && A.x == B.x);
}

Pos add(const Pos A, const Pos B) {
    return makePos(A.y + B.y, A.x + B.x);
}
Pos sub(const Pos A, const Pos B) {
    return makePos(A.y - B.y, A.x - B.x);
}
pos_t distance(const Pos A, const Pos B) {
    Pos del = makePos(A.y - B.y, A.x - B.x);
    return sqrt(del.x * del.x + del.y * del.y);
}
Pos DIRC_TO_POS(const DIRCTION dirc) {
    switch(dirc) {
        case LEFT:  return makePos( 0.,-1.);
        case RIGHT: return makePos( 0., 1.);
        case UP:    return makePos(-1., 0.);
        case DOWN:  return makePos( 1., 0.);
        default:    return makePos( 0., 0.);
    }
}
DIRCTION POS_TO_DIRC(const Pos del) {
    if (del.x == 0 && del.y == 0) return NONE;
    if (fabs(del.x) > fabs(del.y)) {
        if (del.x > 0) return RIGHT;
        else return LEFT;
    } else {
        if (del.y > 0) return DOWN;
        else return UP;
    }
}