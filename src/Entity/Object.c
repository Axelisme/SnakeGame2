
#include "Entity/Object.h"
#include "Interface/Interface.h"

static ALLEGRO_BITMAP* obj_iamge = NULL;

void ObjectClass_init() {
    obj_iamge = nullptr;
}
void ObjectClass_destroy() {
    if (obj_iamge) al_destroy_bitmap(obj_iamge);
}

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
    self->shift = Object_shift;
    self->copy = Object_copy;
    self->deleter = delete_Object;
}
void Object_destroy(Object* self) {
    show_msg("Object_destroy");
}
void delete_Object(Object* self) {
    Object_destroy(self);
    al_free(self);
}
Object* Object_copy(Object* self) {
    Object* new_obj = new_Object(self->pos);
    new_obj->type = self->type;
    new_obj->dir = self->dir;
    new_obj->Image = self->Image;
    return new_obj;
}
static void Object_draw(Object* self, ShiftWindow* sw) {
    if (!SW_isInWindow(sw, self->pos,1)) return;
    Pos UL = SW_getPixelPos(sw, add_const(self->pos, -1));
    Pos LR = SW_getPixelPos(sw, add_const(self->pos,  2));
    ALLEGRO_BITMAP* submap = al_create_sub_bitmap(sw->backbuffer, UL.x, UL.y, LR.x-UL.x, LR.y-UL.y);
    al_set_target_bitmap(submap);
    if (self->Image) draw_image(self->Image, submap, self->dir);
    else al_clear_to_color(RED);
    al_set_target_bitmap(sw->backbuffer);
}
static void Object_shift(Object* self, Pos delta) {
    self->pos = add(self->pos, delta);
}