#ifndef POSITION_H
#define POSITION_H

#include "global.h"

// position
#define pos_t double
typedef struct Pos
{
    pos_t y;
    pos_t x;
} Pos;
Pos makePos(pos_t, pos_t);
bool equal(const Pos, const Pos);
Pos add(const Pos, const Pos);
Pos sub(const Pos, const Pos);
pos_t distance(const Pos, const Pos);
Pos DIRC_TO_POS(const DIRCTION);
DIRCTION POS_TO_DIRC(const Pos);

#endif // POSITION_H