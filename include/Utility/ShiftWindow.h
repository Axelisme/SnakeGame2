#ifndef SHIFTWINDOW_H
#define SHIFTWINDOW_H

#include "global.h"
#include "Utility/Position.h"

typedef struct ShiftWindow {
    // total size
    Pos total_size;
    // view boundary
    Pos view_size;
    Pos view_UL;
    Pos view_LR;
    // shift window
    Pos window_size;
    Pos upperLeft;
    Pos lowerRight;
} ShiftWindow;

void ShiftWindow_init(ShiftWindow* self, Pos total_size, Pos view_UL, Pos view_LR);
void ShiftWindow_destroy(ShiftWindow* self);
void SW_window_resize(ShiftWindow* self, Pos window_size);
void SW_shift(ShiftWindow* self, Pos shift);
void SW_setCenter(ShiftWindow* self, Pos center);
bool SW_isInWindow(ShiftWindow* self, Pos pos, pos_t margin);
Pos  SW_getShiftPos(ShiftWindow* self, Pos pos);
Pos  SW_getPixelPos(ShiftWindow* self, Pos pos, ALLEGRO_BITMAP* backbuffer);

#endif // SHIFTWINDOW_H