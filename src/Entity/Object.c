
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
    if (self->Image) al_destroy_bitmap(self->Image);
}
void delete_Object(Object* self) {
    Object_destroy(self);
    al_free(self);
}
void Object_draw(Object* self, ShiftWindow* sw) {
    if (!self->Image) return;
    if (!SW_isInWindow(sw, self->pos)) return;
    Pos UL = SW_getPixelPos(sw, add_const(self->pos, -1));
    Pos LR = SW_getPixelPos(sw, add_const(self->pos, 2));
    ALLEGRO_BITMAP* submap = al_create_sub_bitmap(sw->backbuffer, UL.x, UL.y, LR.x-UL.x, LR.y-UL.y);
    al_set_target_bitmap(submap);
    if (self->Image) draw_image(self->Image, submap, self->dir);
    else al_clear_to_color(RED);
    al_set_target_bitmap(sw->backbuffer);
}