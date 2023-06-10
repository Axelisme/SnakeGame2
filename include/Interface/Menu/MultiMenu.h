#ifndef MULTI_MENU_H
#define MULTI_MENU_H

#include "Interface/Interface.h"

typedef struct MultiMenu {
    // Inherited from Interface
    Interface interface;
    // Info
    int menu_state;
    // Image
    int image_num;
    ALLEGRO_BITMAP** images;
    // method
    void (*enter_state)(struct MultiMenu* self);
} MultiMenu;

MultiMenu* new_MultiMenu(const char** image_paths, int image_num);
void MultiMenu_init(MultiMenu* self, const char** image_paths, int image_num);
void MultiMenu_destroy(MultiMenu* self);
void delete_MultiMenu(Interface* Iself);
void MultiMenu_draw(Interface* Iself, ALLEGRO_BITMAP* backbuffer);
INTERFACE_INFO MultiMenu_update(Interface* Iself);
void MultiMenu_event_record(Interface* Iself, ALLEGRO_EVENT event);
void MultiMenu_deal_event(Interface* Iself);

void _MultiMenu_init_image(MultiMenu* self, int image_num);
void _MultiMenu_load_image(MultiMenu* self, const char** image_paths, int image_num);
ALLEGRO_BITMAP* _MultiMenu_current_image(MultiMenu* self);
int _MultiMenu_next_state(MultiMenu* self);
int _MultiMenu_prev_state(MultiMenu* self);
void _MultiMenu_enter_state(MultiMenu* self);

#endif // MULTI_MENU_H