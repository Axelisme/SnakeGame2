#ifndef IN_MENU_H
#define IN_MENU_H

#include "Interface/Interface.h"

typedef struct InMenu {
    // inherited from Interface
    Interface interface;
    // in menu image
    ALLEGRO_BITMAP *  in_menu_image;
} Menu;

Menu* new_InMenu();
void InMenu_init(Menu* self);
void INMenu_destroy(Menu* self);
void delete_InMenu(Interface* Iself);
void InMenu_draw(Interface* Iself, ALLEGRO_BITMAP* backbuffer);
INTERFACE_STATE InMenu_update(Interface* Iself);
void InMenu_event_record(Interface* Iself, ALLEGRO_EVENT event);

void _InMenu_deal_event(Menu* self);
void _InMenu_init_image(Menu* self);
void _InMenu_load_image(Menu* self);

#endif // MENU_H