
#include "Utility/ShiftWindow.h"

void ShiftWindow_init(ShiftWindow* self, Pos total_size, Pos view_UL, Pos view_LR) {
    self->total_size = total_size;
    self->view_UL = view_UL;
    self->view_LR = view_LR;
    self->view_size = sub(view_LR, view_UL);
    self->upperLeft = view_UL;
    self->lowerRight = view_LR;
    self->window_size = self->view_size;
}
void ShiftWindow_destroy(ShiftWindow* self) {
    return;
}
void SW_window_resize(ShiftWindow* self, Pos window_size) {
    Pos next_window_size = window_size;
    if (window_size.x > self->view_size.x)
        next_window_size.x = self->view_size.x;
    if (window_size.y > self->view_size.y)
        next_window_size.y = self->view_size.y;
    Pos dsize_half = mul_const(sub(next_window_size, self->window_size), 0.5);
    self->upperLeft = sub(self->upperLeft, dsize_half);
    self->lowerRight = add(self->lowerRight, dsize_half);
    self->window_size = next_window_size;
}
void SW_shift(ShiftWindow* self, Pos shift) {
    // do not shift out of the view boundary
    Pos next_upperLeft = add(self->upperLeft, shift);
    Pos next_lowerRight = add(self->lowerRight, shift);
    if (next_upperLeft.x < self->view_UL.x) {
        next_upperLeft.x = self->view_UL.x;
        next_lowerRight.x = next_upperLeft.x + self->window_size.x;
    }
    if (next_upperLeft.y < self->view_UL.y) {
        next_upperLeft.y = self->view_UL.y;
        next_lowerRight.y = next_upperLeft.y + self->window_size.y;
    }
    if (next_lowerRight.x > self->view_LR.x) {
        next_lowerRight.x = self->view_LR.x;
        next_upperLeft.x = next_lowerRight.x - self->window_size.x;
    }
    if (next_lowerRight.y > self->view_LR.y) {
        next_lowerRight.y = self->view_LR.y;
        next_upperLeft.y = next_lowerRight.y - self->window_size.y;
    }
    self->upperLeft = next_upperLeft;
    self->lowerRight = next_lowerRight;
}
void SW_setCenter(ShiftWindow* self, Pos center) {
    Pos origin_center = mul_const(add(self->lowerRight, self->upperLeft), 0.5);
    Pos shift = sub(center, origin_center);
    SW_shift(self, shift);
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
    const Pos TotalSize = make_Pos(al_get_bitmap_width(backbuffer), al_get_bitmap_height(backbuffer));
    const Pos TileSize = divd(TotalSize, self->window_size);
    const Pos shiftPos = SW_getShiftPos(self, pos);
    return mul(shiftPos, TileSize);
}