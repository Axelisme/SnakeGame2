
#include "Interface/Menu/InMenu.h"

const char IN_MENU_IMAGE_PATH[] = "data/image/menu/In_menu.png";
//const char IN_MENU_IMAGE_PATH[] = "data/image/test2.png";

static const int running_time_light_min = 150;
static const int running_time_light_step = 1;
static int direction = -1;

InMenu* new_InMenu() {
    InMenu* self = new_SingleMenu(IN_MENU_IMAGE_PATH);
    InMenu_init(self);
    return self;
}
void InMenu_init(InMenu* self) {
    if (self == nullptr) {raise_err("try to init NULL interface");return;}
    show_msg("InMenu_init");
    // inherited from Interface
    Interface* Iself = (Interface*)self;
    Iself->info.type = INTERFACE_IN_MENU;
    Iself->background_light = MAX_LIGHT;
    Iself->background_light_down_step = 2;
    Iself->event_dealer = _InMenu_deal_event;
}
void _InMenu_deal_event(Interface* Iself) {
    // breathe light
    if (direction < 0) {
        Iself->background_light -= running_time_light_step;
        if (Iself->background_light < running_time_light_min) {
            Iself->background_light = running_time_light_min;
            direction = 1;
        }
    } else {
        Iself->background_light += running_time_light_step;
        if (Iself->background_light > MAX_LIGHT) {
            Iself->background_light = MAX_LIGHT;
            direction = -1;
        }
    }

    if (Iself->event.type != ALLEGRO_EVENT_KEY_DOWN) return;
    Interface_set_kill(Iself, INTERFACE_START_MENU);
    Iself->event.type = NO_EVENT;
}