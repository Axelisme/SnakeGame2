#ifndef IN_MENU_H
#define IN_MENU_H

#include "Interface/Menu/SingleMenu.h"

SingleMenu* new_InMenu();
void InMenu_init(SingleMenu* self);
void InMenu_deal_event(Interface* Iself);

#endif // MENU_H