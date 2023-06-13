
#include "Entity/Object.h"
#include "Interface/Interface.h"

Object* new_Object(Pos pos) {
    Object* object = (Object*) al_calloc(1, sizeof(Object));
    Object_init(object, pos);
    return object;
}
void Object_init(Object* self, Pos pos) {
    show_msg("Object_init");
    self->type = O_BASIC;
    self->pos = pos;
    self->dir = DIRECTION_UP;
    self->Image = nullptr;
    self->draw = Object_draw;
    self->deleter = delete_Object;
}
void Object_destroy(Object* self) {
    show_msg("Object_destroy");
    if (self->Image != nullptr) al_destroy_bitmap(self->Image);
}
void delete_Object(Object* self) {
    Object_destroy(self);
    al_free(self);
}
void Object_draw(Object* self, ALLEGRO_BITMAP* backbuffer) {
    draw_image(self->Image, backbuffer, self->dir);
}