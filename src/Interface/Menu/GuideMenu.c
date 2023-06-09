
#include "Interface/Menu/GuideMenu.h"

const char GUIDE_MENU_IMAGE_PATH[] = "data/image/menu/guide_menu.png";

SingleMenu* new_GuideMenu() {
    SingleMenu* self = new_SingleMenu(GUIDE_MENU_IMAGE_PATH);
    GuideMenu_init(self);
    return self;
}
void GuideMenu_init(SingleMenu* self) {
    if (self == nullptr) {raise_err("try to init NULL interface");return;}
    show_msg("GuideMenu_init");
    // inherited from Interface
    Interface* Iself = (Interface*)self;
    Iself->info.type = INTERFACE_GUIDE_MENU;
    Iself->event_dealer = GuideMenu_deal_event;
}
void GuideMenu_deal_event(Interface* Iself) {
    SingleMenu* self = (SingleMenu*)Iself;
    if (Iself->event.type == NO_EVENT) return;
    if (Iself->event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (Iself->event.keyboard.keycode) {
            case ALLEGRO_KEY_G:
            case ALLEGRO_KEY_ESCAPE:
                Iself->info.state = INTERFACE_EXITING;
                Iself->info.child.next_interface = INTERFACE_NONE;
                Iself->should_kill = true;
                break;
            default:
                break;
        }
    }
}