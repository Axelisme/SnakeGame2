
#include "Utility/ShiftWindow.h"
#include "Interface/Interface.h"

void ShiftWindow_init(ShiftWindow* self, Pos total_size, Pos view_UL, Pos view_LR) {
    self->total_size = total_size;
    self->view_UL = view_UL;
    self->view_LR = view_LR;
    self->view_size = sub(view_LR, view_UL);
    self->window_UL = view_UL;
    self->window_LR = view_LR;
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
    self->window_UL = sub(self->window_UL, dsize_half);
    self->window_LR = add(self->window_LR, dsize_half);
    self->window_size = next_window_size;
}
void SW_shift(ShiftWindow* self, Pos shift) {
    // do not shift out of the view boundary
    Pos next_upperLeft = add(self->window_UL, shift);
    Pos next_lowerRight = add(self->window_LR, shift);
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
    self->window_UL = next_upperLeft;
    self->window_LR = next_lowerRight;
}
void SW_setCenter(ShiftWindow* self, Pos center) {
    Pos origin_center = mul_const(add(self->window_LR, self->window_UL), 0.5);
    Pos shift = sub(center, origin_center);
    SW_shift(self, shift);
}
bool SW_isInWindow(ShiftWindow* self, Pos pos, pos_t margin) {
    // including the lowerRight but not including the upperLeft
    return (pos.x >= self->window_UL.x-margin && pos.x < self->window_LR.x+margin &&
            pos.y >= self->window_UL.y-margin && pos.y < self->window_LR.y+margin);
}
Pos  SW_getShiftPos(ShiftWindow* self, Pos pos) {
    return sub(pos, self->window_UL);
}
Pos  SW_getPixelPos(ShiftWindow* self, Pos pos, ALLEGRO_BITMAP* backbuffer) {
    const Pos TotalSize = make_Pos(al_get_bitmap_width(backbuffer), al_get_bitmap_height(backbuffer));
    const Pos TileSize = divd(TotalSize, self->window_size);
    const Pos shiftPos = SW_getShiftPos(self, pos);
    return mul(shiftPos, TileSize);
}
void SW_draw_background(ShiftWindow* self, ALLEGRO_BITMAP* backbuffer, ALLEGRO_BITMAP* background, Pos scale) {
    // only need to draw the part in shift window
    const Pos background_UL = add(mul(sub(self->view_UL, self->window_UL), scale), self->window_UL);
    const Pos background_LR = add(mul(sub(self->view_LR, self->window_LR), scale), self->window_LR);
    const Pos bg_UL_pixel = SW_getPixelPos(self, background_UL, backbuffer);
    const Pos bg_LR_pixel = SW_getPixelPos(self, background_LR, backbuffer);
    const Pos bg_pixel_size = sub(bg_LR_pixel, bg_UL_pixel);
    ALLEGRO_BITMAP* view_bitmap = al_create_sub_bitmap(backbuffer, bg_UL_pixel.x, bg_UL_pixel.y, bg_pixel_size.x, bg_pixel_size.y);
    draw_image(background, view_bitmap, DIRECTION_UP, NO_TRANSPARENT);
}