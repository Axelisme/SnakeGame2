
#include "Interface/Menu/StartMenu.h"
#include "GameWindow.h"

#define START_MENU_IMAGE_NUM 5
const char START_MENU_START_IMAGE_PATH[] = "data/image/menu/game_menu_start.png";
const char START_MENU_LEVEL_IMAGE_PATH[] = "data/image/menu/game_menu_level.png";
const char START_MENU_MUSIC_IMAGE_PATH[] = "data/image/menu/game_menu_music.png";
const char START_MENU_GUIDE_IMAGE_PATH[] = "data/image/menu/game_menu_guide.png";
const char START_MENU_EXIT_IMAGE_PATH[]  = "data/image/menu/game_menu_exit.png";

StartMenu* new_StartMenu() {
    const char* IMAGE_PATHS[] = {START_MENU_START_IMAGE_PATH,
                                    START_MENU_LEVEL_IMAGE_PATH,
                                    START_MENU_MUSIC_IMAGE_PATH,
                                    START_MENU_GUIDE_IMAGE_PATH,
                                    START_MENU_EXIT_IMAGE_PATH};
    StartMenu* self = (StartMenu*)new_MultiMenu(IMAGE_PATHS, START_MENU_IMAGE_NUM);
    StartMenu_init(self);
    return self;
}
void StartMenu_init(StartMenu* self) {
    if (self == nullptr) {raise_warn("try to init NULL interface");return;}
    show_msg("StartMenu_init");
    // inherited from Interface
    Interface* Iself = (Interface*)self;
    Iself->info.type = INTERFACE_START_MENU;
    Iself->background_light_up_step = 10;
    // Info
    self->menu_state = START_MENU_STATE_START;
    // method
    self->enter_state = _StartMenu_enter_state;
}

static void _StartMenu_enter_state(StartMenu* self) {
    Interface* Iself = (Interface*)self;
    if (self->menu_state == START_MENU_STATE_START) return;
    switch (self->menu_state) {
        case START_MENU_STATE_LEVEL:
            Interface_set_stop(Iself, INTERFACE_LEVEL_MENU);
            break;
        case START_MENU_STATE_MUSIC:
            GameWindow_toggle_mute();
            break;
        case START_MENU_STATE_GUIDE:
            Interface_set_stop(Iself, INTERFACE_GUIDE_MENU);
            break;
        case START_MENU_STATE_EXIT:
            Interface_set_kill(Iself, INTERFACE_NONE);
            break;
        default:
            raise_warn("unknown state");
            break;
    }
}