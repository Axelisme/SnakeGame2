#ifndef GUIDE_MENU_H
#define GUIDE_MENU_H

#include "Interface/Menu/SingleMenu.h"

typedef SingleMenu GuideMenu;

GuideMenu* new_GuideMenu();
void GuideMenu_init(GuideMenu* self);
void GuideMenu_deal_event(Interface* self);

#endif // GUIDE_MENU_H