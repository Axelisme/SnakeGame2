
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
    Iself->type = INTERFACE_IN_MENU;
    Iself->background_light = MAX_LIGHT;
    Iself->background_light_down_step = 2;
    Iself->draw = InMenu_draw;
    Iself->update = InMenu_update;
    Iself->event_record = InMenu_event_record;
    Iself->deleter = delete_InMenu;
    // in menu image
    self->in_menu_image = nullptr;
    _InMenu_load_image(self);
    if (self->in_menu_image == nullptr) {
        raise_warn("failed to load in menu image!");
        self->interface.state = INTERFACE_DIED;
    }
}
void INMenu_destroy(Menu* self) {
    show_msg("INMenu_destroy");
    // inherited from Interface
    Interface* Iself = (Interface*)self;
    Interface_destroy(Iself);
    // free in menu image
    _InMenu_init_image(self);
}
void delete_InMenu(Interface* Iself) {
    Menu* self = (Menu*)Iself;
    INMenu_destroy(self);
    al_free(self);
}
void InMenu_draw(Interface* Iself, ALLEGRO_BITMAP* backbuffer) {
    if (Iself == nullptr) {raise_warn("try to draw NULL interface");return;}
    if (backbuffer == nullptr) {raise_warn("try to draw interface on NULL backbuffer");return;}
    Menu* self = (Menu*)Iself;
    if (Iself->state == INTERFACE_DIED) return;
    Interface_draw(Iself, backbuffer);
    _draw_image(self->in_menu_image, backbuffer);
}
INTERFACE_STATE InMenu_update(Interface* Iself) {
    if (Iself == nullptr) {raise_warn("try to update NULL interface");return INTERFACE_DIED;}
    Menu* self = (Menu*)Iself;
    if (Iself->state == INTERFACE_DIED) return INTERFACE_DIED;
    switch (Iself->state) {
        case INTERFACE_INITIALING:
        case INTERFACE_STOP:
            Iself->state = INTERFACE_RUNING;
            break;
        case INTERFACE_RUNING:
            _InMenu_deal_event(self);
            break;
        case INTERFACE_EXITING:
            if (_Interface_update_light(Iself, -1))
                Iself->state = INTERFACE_DIED;
            break;
        case INTERFACE_DIED:
            break;
        default:
            raise_err("unknown interface state");
            break;
    }
    return Iself->state;
}
void InMenu_event_record(Interface* Iself, ALLEGRO_EVENT event) {
    if (Iself == nullptr) {raise_warn("try to record event on NULL interface");return;}
    Iself->event.type = -1;
    switch (Iself->state) {
        case INTERFACE_INITIALING:
        case INTERFACE_RUNING:
        case INTERFACE_EXITING:
            Iself->event = event;
            break;
        case INTERFACE_STOP:
        case INTERFACE_DIED:
            raise_warn("try to record event on stop or died interface");
            break;
        default:
            raise_warn("unknown interface state while recording event");
            break;
    }
}

void _InMenu_deal_event(Menu* self) {
    if (self == nullptr) {raise_warn("try to deal event on NULL interface");return;}
    Interface* Iself = (Interface*)self;
    if (Iself->state == INTERFACE_DIED) return;
    switch (Iself->event.type) {
        case ALLEGRO_EVENT_KEY_DOWN:
            Iself->state = INTERFACE_EXITING;
            Iself->next_interface = INTERFACE_START_MENU;
            break;
        default:
            break;
    }
}
void _InMenu_init_image(Menu* self) {
    if (self->in_menu_image != nullptr) {
        al_destroy_bitmap(self->in_menu_image);
        self->in_menu_image = nullptr;
    }
}
void _InMenu_load_image(Menu* self) {
    show_msg("_InMenu_load_image");
    _InMenu_init_image(self);
    self->in_menu_image = al_load_bitmap(IN_MENU_IMAGE_PATH);
}