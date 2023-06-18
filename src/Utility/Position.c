
#undef __STRICT_ANSI__
#include "Utility/Position.h"
#include "math.h"

Pos make_Pos(pos_t y, pos_t x) {
    return (Pos) {y, x};
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
Pos mul(const Pos A, const Pos B) {
    return make_Pos(A.y * B.y, A.x * B.x);
}
Pos divd(const Pos A, const Pos B) {
    return make_Pos(A.y / B.y, A.x / B.x);
}
Pos add_const(const Pos A, const pos_t B) {
    return make_Pos(A.y + B, A.x + B);
}
Pos mul_const(const Pos A, const pos_t B) {
    return make_Pos(A.y * B, A.x * B);
}
Pos neighbor_pos(const Pos pos, const Direction dir) {
    return add(pos, DIR_TO_POS(dir));
}
Pos DIR_TO_POS(const Direction dir) {
    switch(dir) {
        case DIRECTION_LEFT:  return make_Pos( 0.,-1.);
        case DIRECTION_RIGHT: return make_Pos( 0., 1.);
        case DIRECTION_UP:    return make_Pos(-1., 0.);
        case DIRECTION_DOWN:  return make_Pos( 1., 0.);
        default:             return make_Pos( 0., 0.);
    }
}
Direction POS_TO_DIR(const Pos pos) {
    if (pos.x == 0 && pos.y == 0) return DIRECTION_NONE;
    if (fabs(pos.x) > fabs(pos.y)) {
        if (pos.x > 0) return DIRECTION_RIGHT;
        else return DIRECTION_LEFT;
    } else {
        if (pos.y > 0) return DIRECTION_DOWN;
        else return DIRECTION_UP;
    }
}
Pos POS_TO_PIXEL(const Pos pos, ALLEGRO_BITMAP* bitmap) {
    return make_Pos(pos.y * al_get_bitmap_height(bitmap), pos.x * al_get_bitmap_width(bitmap));
}