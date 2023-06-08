
#include "Interface/Menu/GuideMenu.h"

const char GUIDE_MENU_IMAGE_PATH[] = "data/image/menu/guide_menu.png";

GuideMenu* new_GuideMenu() {
    GuideMenu* self = (GuideMenu*)al_calloc(1, sizeof(GuideMenu));
    GuideMenu_init(self);
    return self;
}
void GuideMenu_init(GuideMenu* self) {
    if (self == nullptr) {raise_err("try to init NULL interface");return;}
    show_msg("GuideMenu_init");
    // inherited from Interface
    Interface* Iself = (Interface*)self;
    Interface_init(Iself);
    Iself->info.type = INTERFACE_GUIDE_MENU;
    Iself->background_light = MIN_LIGHT;
    Iself->should_kill = true;
    Iself->draw = GuideMenu_draw;
    Iself->update = GuideMenu_update;
    Iself->event_record = Interface_event_record;
    Iself->deleter = delete_GuideMenu;
    // in menu image
    self->guide_menu_image = nullptr;
    _GuideMenu_load_image(self);
    if (self->guide_menu_image == nullptr) {
        raise_warn("failed to load guide menu image!");
        Iself->info.state = INTERFACE_DIED;
    }
}
void GuideMenu_destroy(GuideMenu* self) {
    show_msg("GuideMenu_destroy");
    // inherited from Interface
    Interface* Iself = (Interface*)self;
    Interface_destroy(Iself);
    // free in menu image
    _GuideMenu_init_image(self);
}
void delete_GuideMenu(Interface* Iself) {
    GuideMenu* self = (GuideMenu*)Iself;
    GuideMenu_destroy(self);
    al_free(self);
}
void GuideMenu_draw(Interface* Iself, ALLEGRO_BITMAP* backbuffer) {
    if (Iself == nullptr) {raise_warn("try to draw NULL interface");return;}
    if (backbuffer == nullptr) {raise_warn("try to draw interface on NULL backbuffer");return;}
    GuideMenu* self = (GuideMenu*)Iself;
    if (Iself->info.state == INTERFACE_DIED) return;
    Interface_draw(Iself, backbuffer);
    if (self->guide_menu_image) draw_image(self->guide_menu_image, backbuffer);
    else raise_warn("try to draw NULL guide image");
}
INTERFACE_INFO GuideMenu_update(Interface* Iself) {
    if (Iself == nullptr) {raise_warn("try to update NULL interface");return _fall_back_info();}
    GuideMenu* self = (GuideMenu*)Iself;
    switch (Iself->info.state) {
        case INTERFACE_INITIALING:
            if (Interface_update_light(Iself,1))
                Iself->info.state = INTERFACE_RUNING;
            break;
        case INTERFACE_STOP:
            Iself->info.state = INTERFACE_INITIALING;
            break;
        case INTERFACE_RUNING:
            _GuideMenu_deal_event(self);
            break;
        case INTERFACE_EXITING:
            if (Interface_update_light(Iself,-1))
                Iself->info.state = (Iself->should_kill) ? INTERFACE_DIED : INTERFACE_STOP;
            break;
        case INTERFACE_DIED:
            break;
        default:
            break;
    }
    return Iself->info;
}

static void _GuideMenu_deal_event(GuideMenu* self) {
    Interface* Iself = (Interface*)self;
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
static void _GuideMenu_init_image(GuideMenu* self) {
    if (self->guide_menu_image) al_destroy_bitmap(self->guide_menu_image);
    self->guide_menu_image = nullptr;
}
static void _GuideMenu_load_image(GuideMenu* self) {
    _GuideMenu_init_image(self);
    self->guide_menu_image = al_load_bitmap(GUIDE_MENU_IMAGE_PATH);
    if (self->guide_menu_image == nullptr)
        raise_warn("failed to load guide menu image!");
}