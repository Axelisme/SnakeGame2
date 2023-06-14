#ifndef LEVEL_WIN_H
#define LEVEL_WIN_H

#include "Interface/Menu/SingleMenu.h"

typedef SingleMenu LevelWin;

LevelWin* new_LevelWin();
void LevelWin_init(LevelWin* self);

static void _LevelWin_deal_event(Interface* Iself);

#endif // LEVEL_WIN_H