
#include "Interface/Menu/SingleMenu.h"

SingleMenu* new_SingleMenu(const char* image_path) {
    SingleMenu* self = (SingleMenu*)al_calloc(1, sizeof(SingleMenu));
    SingleMenu_init(self, image_path);
    return self;
}
void SingleMenu_init(SingleMenu* self, const char* image_path) {
    if (self == nullptr) {raise_err("try to init NULL interface");return;}
    Interface* Iself = (Interface*)self;
    Interface_init(Iself);
    show_msg("SingleMenu_init");
    // inherited from Interface
    Iself->info.type = INTERFACE_SINGLE;
    Iself->deleter = delete_SingleMenu;
    Iself->draw = SingleMenu_draw;
    Iself->deleter = delete_SingleMenu;
    // in menu image
    self->image = nullptr;
    _SingleMenu_free_image(self);
    SingleMenu_load_image(self, image_path);
    if (self->image == nullptr) {
        raise_warn("failed to load image! use default draw method");
        Iself->draw = Interface_draw;
    }
}
void SingleMenu_destroy(SingleMenu* self) {
    show_msg("SingleMenu_destroy");
    // free in menu image
    _SingleMenu_free_image(self);
    // inherited from Interface
    Interface* Iself = (Interface*)self;
    Interface_destroy(Iself);
}
void delete_SingleMenu(Interface* Iself) {
    SingleMenu* self = (SingleMenu*)Iself;
    SingleMenu_destroy(self);
    al_free(self);
}
void SingleMenu_draw(Interface* Iself, ALLEGRO_BITMAP* backbuffer) {
    if (Iself == nullptr) {
        raise_warn("try to draw NULL interface");return;}
    if (backbuffer == nullptr) {
        raise_warn("try to draw interface on NULL backbuffer");return;}
    SingleMenu* self = (SingleMenu*)Iself;
    if (Iself->info.state == INTERFACE_DIED) return;
    Interface_draw(Iself, backbuffer);
    if (self->image) draw_image(self->image, backbuffer, DIRECTION_UP, NO_TRANSPARENT);
    else raise_warn("try to draw NULL image");
}
void SingleMenu_load_image(SingleMenu* self, const char* image_path) {
    self->image = al_load_bitmap(image_path);
}

static void _SingleMenu_free_image(SingleMenu* self) {
    if (self->image) al_destroy_bitmap(self->image);
    self->image = nullptr;
}