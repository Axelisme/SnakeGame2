
#include "Interface/Menu/MultiMenu.h"

MultiMenu* new_MultiMenu(const char** image_paths, int image_num) {
    MultiMenu* self = (MultiMenu*)al_calloc(1, sizeof(MultiMenu));
    MultiMenu_init(self, image_paths, image_num);
    return self;
}
void MultiMenu_init(MultiMenu* self, const char** image_paths, int image_num) {
    if (self == nullptr) {
        raise_warn("try to init a nullptr MultiMenu"); return;}
    Interface* Iself = (Interface*)self;
    Interface_init(Iself);
    show_msg("MultiMenu_init");
    // inherited from Interface
    Iself->info.type = INTERFACE_MULTI;
    Iself->draw = MultiMenu_draw;
    Iself->event_record = MultiMenu_event_record;
    Iself->event_dealer = MultiMenu_deal_event;
    Iself->deleter = delete_MultiMenu;
    // Info
    self->menu_state = 0;
    // Image
    self->image_num = image_num;
    self->images = (ALLEGRO_BITMAP**)al_calloc(image_num, sizeof(ALLEGRO_BITMAP*));
    _MultiMenu_free_image(self, image_num);
    _MultiMenu_load_image(self, image_paths, image_num);
    // method
    self->enter_state = _MultiMenu_enter_state;
}
void MultiMenu_destroy(MultiMenu* self) {
    show_msg("MultiMenu_destroy");
    // free image
    _MultiMenu_free_image(self, self->image_num);
    al_free(self->images);
    // inherited from Interface
    Interface* Iself = (Interface*)self;
    Interface_destroy(Iself);
}
void delete_MultiMenu(Interface* Iself) {
    MultiMenu* self = (MultiMenu*)Iself;
    MultiMenu_destroy(self);
    al_free(self);
}
void MultiMenu_draw(Interface* Iself, ALLEGRO_BITMAP* backbuffer) {
    MultiMenu* self = (MultiMenu*)Iself;
    if (self == nullptr) {
        raise_warn("try to draw NULL interface");return;}
    if (backbuffer == nullptr) {
        raise_warn("try to draw interface on NULL backbuffer");return;}
    if (Iself->info.state == INTERFACE_DIED) return;
    // inherited from Interface
    Interface_draw(Iself, backbuffer);
    // draw image
    ALLEGRO_BITMAP* image = _MultiMenu_current_image(self);
    if (image) draw_image(image, backbuffer, DIRECTION_UP, NO_TRANSPARENT);
}
void MultiMenu_event_record(Interface* Iself, ALLEGRO_EVENT event) {
    MultiMenu* self = (MultiMenu*)Iself;
    if (self == nullptr) {raise_warn("try to record event on NULL interface");return;}
    if (Iself->info.state != INTERFACE_RUNNING && Iself->info.state != INTERFACE_INITIALING) return;
    if (event.type != ALLEGRO_EVENT_KEY_DOWN) return;
    switch (event.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
        case ALLEGRO_KEY_DOWN:
        case ALLEGRO_KEY_ENTER:
        case ALLEGRO_KEY_SPACE:
        case ALLEGRO_KEY_ESCAPE:
            Iself->event = event;
            break;
        default:
            break;
    }
}
void MultiMenu_deal_event(Interface* Iself) {
    MultiMenu* self = (MultiMenu*)Iself;
    if (Iself->event.type == NO_EVENT) return;
    switch (Iself->event.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
            show_msg("MultiMenu event: up");
            self->menu_state = _MultiMenu_prev_state(self);
            break;
        case ALLEGRO_KEY_DOWN:
            show_msg("MultiMenu event: down");
            self->menu_state = _MultiMenu_next_state(self);
            break;
        case ALLEGRO_KEY_SPACE:
        case ALLEGRO_KEY_ENTER:
            show_msg("MultiMenu event: enter");
            self->enter_state(self);
            break;
        case ALLEGRO_KEY_ESCAPE:
            show_msg("MultiMenu event: escape");
            Interface_set_kill(Iself, INTERFACE_NONE);
            break;
        default:
            raise_warn("try to deal with unknown event");
            break;
    }
    Iself->event.type = NO_EVENT;
}

void _MultiMenu_free_image(MultiMenu* self, int image_num) {
    for (int i = 0; i < image_num; i++) {
        if (self->images[i] != nullptr)
            al_destroy_bitmap(self->images[i]);
        self->images[i] = nullptr;
    }
}
void _MultiMenu_load_image(MultiMenu* self, const char** image_paths, int image_num) {
    for (int i = 0; i < image_num; i++) {
        self->images[i] = al_load_bitmap(image_paths[i]);
        if (self->images[i] == nullptr)
            raise_warn("fail to load image");
    }
}
ALLEGRO_BITMAP* _MultiMenu_current_image(MultiMenu* self) {
    return self->images[self->menu_state];
}
int _MultiMenu_next_state(MultiMenu* self) {
    return (self->menu_state+1 == self->image_num) ? self->menu_state : self->menu_state + 1;
}
int _MultiMenu_prev_state(MultiMenu* self) {
    return (self->menu_state == 0) ? self->menu_state : self->menu_state - 1;
}
void _MultiMenu_enter_state(MultiMenu* self) {
    return;
}
