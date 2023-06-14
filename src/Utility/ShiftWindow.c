
#include "Utility/ShiftWindow.h"

void ShiftWindow_init(ShiftWindow* self, Pos size, ALLEGRO_BITMAP* backbuffer) {
    self->total_size = size;
    self->upperLeft = make_Pos(0, 0);
    self->lowerRight = size;
    self->window_size = sub(self->lowerRight, self->upperLeft);
    self->backbuffer = backbuffer;
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
bool SW_isInWindow(ShiftWindow* self, Pos pos, pos_t margin) {
    // including the lowerRight but not including the upperLeft
    return (pos.x >= self->upperLeft.x-margin && pos.x < self->lowerRight.x+margin &&
            pos.y >= self->upperLeft.y-margin && pos.y < self->lowerRight.y+margin);
}
Pos  SW_getShiftPos(ShiftWindow* self, Pos pos) {
    return sub(pos, self->upperLeft);
}
Pos  SW_getPixelPos(ShiftWindow* self, Pos pos) {
    int TotalWidth = al_get_bitmap_width(self->backbuffer);
    int TotalHeight = al_get_bitmap_height(self->backbuffer);
    const pos_t WidthPerTile = (pos_t) TotalWidth / self->window_size.x;
    const pos_t HeightPerTile = (pos_t) TotalHeight / self->window_size.y;
    Pos shiftPos = SW_getShiftPos(self, pos);
    return make_Pos(shiftPos.x * WidthPerTile, shiftPos.y * HeightPerTile);
}