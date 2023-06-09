
#include "Interface/Menu/InMenu.h"

const char IN_MENU_IMAGE_PATH[] = "data/image/menu/In_menu.png";

SingleMenu* new_InMenu() {
    SingleMenu* self = new_SingleMenu(IN_MENU_IMAGE_PATH);
    InMenu_init(self);
    return self;
}
void InMenu_init(SingleMenu* self) {
    if (self == nullptr) {raise_err("try to init NULL interface");return;}
    show_msg("InMenu_init");
    // inherited from Interface
    Interface* Iself = (Interface*)self;
    Iself->info.type = INTERFACE_IN_MENU;
    Iself->background_light = MAX_LIGHT;
    Iself->background_light_down_step = 2;
    Iself->event_dealer = InMenu_deal_event;
}
void InMenu_deal_event(Interface* Iself) {
    if (Iself->event.type != ALLEGRO_EVENT_KEY_DOWN) return;
    Iself->info.state = INTERFACE_EXITING;
    Iself->info.child.next_interface = INTERFACE_START_MENU;
    Iself->should_kill = true;
    Iself->event.type = NO_EVENT;
}