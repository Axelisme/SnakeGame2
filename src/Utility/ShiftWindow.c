
#include "Utility/ShiftWindow.h"

void ShiftWindow_init(ShiftWindow* self, Pos size, ALLEGRO_BITMAP* backbuffer) {
    self->size = size;
    self->upperLeft = make_Pos(0, 0);
    self->lowerRight = size;
    self->backbuffer = backbuffer;
}
void ShiftWindow_destroy(ShiftWindow* self) {
    return;
}
void ShiftWindow_resize(ShiftWindow* self, Pos upperLeft, Pos lowerRight) {
    self->upperLeft = upperLeft;
    self->lowerRight = lowerRight;
}
void ShiftWindow_shift(ShiftWindow* self, Pos shift) {
    self->upperLeft = add(self->upperLeft, shift);
    self->lowerRight = add(self->lowerRight, shift);
}
bool SW_isInWindow(ShiftWindow* self, Pos pos) {
    // including the lowerRight but not including the upperLeft
    return (pos.x >= self->upperLeft.x && pos.x < self->lowerRight.x &&
            pos.y >= self->upperLeft.y && pos.y < self->lowerRight.y);
}
Pos  SW_getShiftPos(ShiftWindow* self, Pos pos) {
    return sub(pos, self->upperLeft);
}
Pos  SW_getPixelPos(ShiftWindow* self, Pos pos) {
    int TotalWidth = al_get_bitmap_width(self->backbuffer);
    int TotalHeight = al_get_bitmap_height(self->backbuffer);
    int WidthPerTile = TotalWidth / self->size.x;
    int HeightPerTile = TotalHeight / self->size.y;
    Pos shiftPos = SW_getShiftPos(self, pos);
    return make_Pos(shiftPos.x * WidthPerTile, shiftPos.y * HeightPerTile);
}