#ifndef IN_MENU_H
#define IN_MENU_H

#include "Interface/Menu/SingleMenu.h"

typedef SingleMenu InMenu;

InMenu* new_InMenu();
void InMenu_init(InMenu* self);

static void _InMenu_deal_event(Interface* Iself);

#endif // MENU_H