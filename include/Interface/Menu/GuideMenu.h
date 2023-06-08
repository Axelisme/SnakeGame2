#ifndef GUIDE_MENU_H
#define GUIDE_MENU_H

#include "Interface/Interface.h"

typedef struct GuideMenu {
    // inherited from Interface
    Interface interface;
    // in menu image
    ALLEGRO_BITMAP *  guide_menu_image;
} GuideMenu;

GuideMenu* new_GuideMenu();
void GuideMenu_init(GuideMenu* self);
void GuideMenu_destroy(GuideMenu* self);
void delete_GuideMenu(Interface* Iself);
void GuideMenu_draw(Interface* Iself, ALLEGRO_BITMAP* backbuffer);
INTERFACE_INFO GuideMenu_update(Interface* Iself);
void GuideMenu_event_record(Interface* Iself, ALLEGRO_EVENT event);

static void _GuideMenu_deal_event(GuideMenu* self);
static void _GuideMenu_init_image(GuideMenu* self);
static void _GuideMenu_load_image(GuideMenu* self);

#endif // GUIDE_MENU_H