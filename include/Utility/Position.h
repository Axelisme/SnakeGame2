#ifndef POSITION_H
#define POSITION_H

#include <stdbool.h>
#include "Utility/Direction.h"

// position
#define pos_t int
typedef struct Pos {
    pos_t y;
    pos_t x;
} Pos;

Pos make_Pos(pos_t y, pos_t x);
bool equal(const Pos A, const Pos B);
Pos add(const Pos A, const Pos B);
Pos sub(const Pos A, const Pos B);
Pos heading_pos(const Pos pos, const Direction dir);
Pos DIR_TO_POS(const Direction dir);
Direction POS_TO_DIR(const Pos pos);

#endif // POSITION_H