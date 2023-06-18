#ifndef MUSIC_MENU_H
#define MUSIC_MENU_H

#include "Interface/Interface.h"
#include "Interface/Menu/MultiMenu.h"
#include "SoundEngine.h"

typedef enum MusicMenuState {
    MUSIC_MENU_VOLUME,
    MUSIC_MENU_EXIT
} MusicMenuState;

typedef struct MusicMenu {
    // Inherited from Interface
    Interface interface;
    // Info
    bool Mute;
    MusicMenuState menu_state;
    // Image
    ALLEGRO_BITMAP* volume_image;
    ALLEGRO_BITMAP* exit_image;
    ALLEGRO_BITMAP* volume_mute_image;
    ALLEGRO_BITMAP* exit_mute_image;
    ALLEGRO_BITMAP* music_cursor_image;
    ALLEGRO_BITMAP* music_cursor_image_dark;
} MusicMenu;

MusicMenu* new_MusicMenu();
void MusicMenu_init(MusicMenu *music_menu);
void MusicMenu_destroy(MusicMenu* music_menu);
void delete_MusicMenu(Interface* Iself);
void MusicMenu_draw(Interface* Iself, ALLEGRO_BITMAP* backbuffer);
void MusicMenu_event_record(Interface* Iself, ALLEGRO_EVENT event);
void MusicMenu_deal_event(Interface* Iself);

static ALLEGRO_BITMAP* _MusicMenu_current_image(MusicMenu* music_menu);
static void _MusicMenu_enter_state(MusicMenu* music_menu);
static void _MusicMenu_draw_cursor(MusicMenu* music_menu, ALLEGRO_BITMAP* backbuffer);



#endif // MUSIC_MENU_H