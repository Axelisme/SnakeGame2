
#include "Interface/Menu/InMenu.h"

const char IN_MENU_IMAGE_PATH[] = "data/image/menu/In_menu.png";

Menu* new_InMenu() {
    Menu* self = (Menu*)al_calloc(1, sizeof(Menu));
    InMenu_init(self);
    return self;
}
void InMenu_init(Menu* self) {
    if (self == nullptr) {raise_err("try to init NULL interface");return;}
    show_msg("InMenu_init");
    // inherited from Interface
    Interface* Iself = (Interface*)self;
    Interface_init(Iself);
    Iself->info.type = INTERFACE_IN_MENU;
    Iself->background_light = MAX_LIGHT;
    Iself->background_light_down_step = 2;
    Iself->draw = InMenu_draw;
    Iself->update = InMenu_update;
    Iself->event_record = Interface_event_record;
    Iself->deleter = delete_InMenu;
    // in menu image
    self->in_menu_image = nullptr;
    _InMenu_load_image(self);
    if (self->in_menu_image == nullptr) {
        raise_warn("failed to load in menu image!");
        Iself->info.state = INTERFACE_DIED;
    }
}
void InMenu_destroy(Menu* self) {
    show_msg("INMenu_destroy");
    // inherited from Interface
    Interface* Iself = (Interface*)self;
    Interface_destroy(Iself);
    // free in menu image
    _InMenu_init_image(self);
}
void delete_InMenu(Interface* Iself) {
    Menu* self = (Menu*)Iself;
    InMenu_destroy(self);
    al_free(self);
}
void InMenu_draw(Interface* Iself, ALLEGRO_BITMAP* backbuffer) {
    if (Iself == nullptr) {raise_warn("try to draw NULL interface");return;}
    if (backbuffer == nullptr) {raise_warn("try to draw interface on NULL backbuffer");return;}
    Menu* self = (Menu*)Iself;
    if (Iself->info.state == INTERFACE_DIED) return;
    Interface_draw(Iself, backbuffer);
    if (self->in_menu_image) _draw_image(self->in_menu_image, backbuffer);
    else raise_warn("try to draw NULL in menu image");
}
INTERFACE_INFO InMenu_update(Interface* Iself) {
    if (Iself == nullptr) {raise_warn("try to update NULL interface");return _fall_back_info();}
    Menu* self = (Menu*)Iself;
    switch (Iself->info.state) {
        case INTERFACE_INITIALING:
        case INTERFACE_STOP:
            Iself->info.state = INTERFACE_RUNING;
            break;
        case INTERFACE_RUNING:
            _InMenu_deal_event(self);
            break;
        case INTERFACE_EXITING:
            if (_Interface_update_light(Iself, -1))
                Iself->info.state = (Iself->should_kill)? INTERFACE_DIED: INTERFACE_STOP;
            break;
        case INTERFACE_DIED:
            break;
        default:
            raise_err("unknown interface state");
            break;
    }
    return Iself->info;
}

void _InMenu_deal_event(Menu* self) {
    Interface* Iself = (Interface*)self;
    if (Iself->event.type == NO_EVENT) return;
    if (Iself->event.type == ALLEGRO_EVENT_KEY_DOWN) {
        Iself->info.state = INTERFACE_EXITING;
        Iself->info.child.next_interface = INTERFACE_START_MENU;
        Iself->should_kill = true;
    }
    Iself->event.type = NO_EVENT;
}
void _InMenu_init_image(Menu* self) {
    if (self->in_menu_image) al_destroy_bitmap(self->in_menu_image);
    self->in_menu_image = nullptr;
}
void _InMenu_load_image(Menu* self) {
    _InMenu_init_image(self);
    self->in_menu_image = al_load_bitmap(IN_MENU_IMAGE_PATH);
    if (self->in_menu_image == nullptr)
        raise_warn("failed to load in menu image!");
}