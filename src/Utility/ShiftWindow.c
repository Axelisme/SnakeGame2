
#include "Utility/ShiftWindow.h"

void ShiftWindow_init(ShiftWindow* self, Pos size) {
    self->total_size = size;
    self->upperLeft = make_Pos(0, 0);
    self->lowerRight = size;
    self->window_size = sub(self->lowerRight, self->upperLeft);
}
void ShiftWindow_destroy(ShiftWindow* self) {
    return;
}
void ShiftWindow_resize(ShiftWindow* self, Pos upperLeft, Pos lowerRight) {
    self->upperLeft = upperLeft;
    self->lowerRight = lowerRight;
    self->window_size = sub(lowerRight, upperLeft);
}
void ShiftWindow_shift(ShiftWindow* self, Pos shift) {
    self->upperLeft = add(self->upperLeft, shift);
    self->lowerRight = add(self->lowerRight, shift);
}
void ShiftWindow_setCenter(ShiftWindow* self, Pos center) {
    Pos origin_center = mul_const(add(self->lowerRight, self->upperLeft), 0.5);
    Pos shift = sub(center, origin_center);
    self->upperLeft = add(self->upperLeft, shift);
    self->lowerRight = add(self->lowerRight, shift);
}
bool SW_isInWindow(ShiftWindow* self, Pos pos, pos_t margin) {
    // including the lowerRight but not including the upperLeft
    return (pos.x >= self->upperLeft.x-margin && pos.x < self->lowerRight.x+margin &&
            pos.y >= self->upperLeft.y-margin && pos.y < self->lowerRight.y+margin);
}
Pos  SW_getShiftPos(ShiftWindow* self, Pos pos) {
    return sub(pos, self->upperLeft);
}
Pos  SW_getPixelPos(ShiftWindow* self, Pos pos, ALLEGRO_BITMAP* backbuffer) {
    int TotalWidth = al_get_bitmap_width(backbuffer);
    int TotalHeight = al_get_bitmap_height(backbuffer);
    const pos_t WidthPerTile = (pos_t) TotalWidth / self->window_size.x;
    const pos_t HeightPerTile = (pos_t) TotalHeight / self->window_size.y;
    Pos shiftPos = SW_getShiftPos(self, pos);
    return make_Pos(shiftPos.x * WidthPerTile, shiftPos.y * HeightPerTile);
}