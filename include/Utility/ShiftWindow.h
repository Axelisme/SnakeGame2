#ifndef SHIFTWINDOW_H
#define SHIFTWINDOW_H

#include "global.h"
#include "Utility/Position.h"

typedef struct ShiftWindow {
    // total size
    Pos total_size;
    // shift window
    Pos window_size;
    Pos upperLeft;
    Pos lowerRight;
    ALLEGRO_BITMAP* backbuffer;
} ShiftWindow;

void ShiftWindow_init(ShiftWindow* self, Pos size, ALLEGRO_BITMAP* backbuffer);
void ShiftWindow_destroy(ShiftWindow* self);
void ShiftWindow_resize(ShiftWindow* self, Pos upperLeft, Pos lowerRight);
void ShiftWindow_shift(ShiftWindow* self, Pos shift);
bool SW_isInWindow(ShiftWindow* self, Pos pos, pos_t margin);
Pos  SW_getShiftPos(ShiftWindow* self, Pos pos);
Pos  SW_getPixelPos(ShiftWindow* self, Pos pos);

#endif // SHIFTWINDOW_H