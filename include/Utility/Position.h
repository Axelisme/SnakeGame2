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

#define fpos_t float
typedef struct FPos {
    fpos_t y;
    fpos_t x;
} FPos;

Pos make_Pos(pos_t y, pos_t x);
bool equal(const Pos A, const Pos B);
Pos add(const Pos A, const Pos B);
Pos sub(const Pos A, const Pos B);
fpos_t distance(const Pos A, const Pos B);
Pos heading_pos(const Pos pos, const DIRECTION dir);
inline Pos DIR_TO_POS(const DIRECTION dir);
inline DIRECTION POS_TO_DIR(const Pos pos);

#endif // POSITION_H