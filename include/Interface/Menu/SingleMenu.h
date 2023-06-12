#ifndef SINGLE_MENU_H
#define SINGLE_MENU_H

#include "Interface/Interface.h"

typedef struct SingleMenu {
    // inherited from Interface
    Interface interface;
    // info
    void * info;
    // in menu image
    ALLEGRO_BITMAP *  image;
} SingleMenu;

SingleMenu* new_SingleMenu(const char* image_path);
void SingleMenu_init(SingleMenu* self, const char* image_path);
void SingleMenu_destroy(SingleMenu* self);
void delete_SingleMenu(Interface* Iself);
void SingleMenu_draw(Interface* Iself, ALLEGRO_BITMAP* backbuffer);
void SingleMenu_load_image(SingleMenu* self, const char* image_path);

static void _SingleMenu_free_image(SingleMenu* self);


#endif // SINGLE_MENU_H