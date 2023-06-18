
#include "Object/BodyObject.h"

BodyObject* new_BodyObject(Pos pos, BodyType type, Direction from, Direction to) {
    BodyObject* self = (BodyObject*)al_calloc(1,sizeof(BodyObject));
    BodyObject_init(self, pos, type, from, to);
    return self;
}
void BodyObject_init(BodyObject* self, Pos pos, BodyType type, Direction from, Direction to) {
    // Inherited from Object
    Object* Oself = (Object*)self;
    Object_init(Oself, pos);
    Oself->Image = snake_head_img;
    Oself->draw = BodyObject_draw;
    Oself->copy = BodyObject_copy;
    Oself->deleter = delete_BodyObject;

    self->type = type;
    self->from = from;
    self->to = to;
    self->object.Image = BodyObject_get_Image(self, &self->object.dir);
}
void BodyObject_destroy(BodyObject* self) {
    // Inherited from Object
    Object* Oself = (Object*)self;
    Object_destroy(Oself);
}
void delete_BodyObject(Object* Oself) {
    BodyObject* self = (BodyObject*)Oself;
    BodyObject_destroy(self);
    al_free(self);
}

static void BodyObject_draw(Object* Oself, ShiftWindow* sw, ALLEGRO_BITMAP* backbuffer) {
    BodyObject* self = (BodyObject*)Oself;
    self->object.Image = BodyObject_get_Image(self, &self->object.dir);
    Object_draw(Oself, sw, backbuffer);
}
static Object* BodyObject_copy(Object* Oself) {
    BodyObject* self = (BodyObject*)Oself;
    BodyObject* new_obj = new_BodyObject(self->object.pos, self->type, self->from, self->to);
    return (Object*)new_obj;
}
static ALLEGRO_BITMAP* BodyObject_get_Image(BodyObject* self, Direction* dir) {
    Object* Oself = (Object*)self;
    Direction from = self->from;
    Direction to = self->to;
    switch (self->type) {
        case HEAD:
            *dir = to;
            return Oself->Image;
        case TAIL:
            *dir = to;
            return snake_tail_img;
        case BODY:
            // Straight case
            if (from == to) {
                *dir = to;
                return snake_body_straight_img;
            }
            // Turn case, origin image from=up and to=right
            // change draw dir to fit any case
            if      (from==DIRECTION_UP    && to==DIRECTION_RIGHT)   *dir = DIRECTION_UP;
            else if (from==DIRECTION_LEFT  && to==DIRECTION_DOWN)    *dir = DIRECTION_UP;
            else if (from==DIRECTION_UP    && to==DIRECTION_LEFT)    *dir = DIRECTION_RIGHT;
            else if (from==DIRECTION_RIGHT && to==DIRECTION_DOWN)    *dir = DIRECTION_RIGHT;
            else if (from==DIRECTION_DOWN  && to==DIRECTION_LEFT)    *dir = DIRECTION_DOWN;
            else if (from==DIRECTION_RIGHT && to==DIRECTION_UP)      *dir = DIRECTION_DOWN;
            else if (from==DIRECTION_DOWN  && to==DIRECTION_RIGHT)   *dir = DIRECTION_LEFT;
            else if (from==DIRECTION_LEFT  && to==DIRECTION_UP)      *dir = DIRECTION_LEFT;
            else
                raise_warn("Body_get_Image: Unknown Body Direction");
            return snake_body_turn_img;
        default:
            raise_warn("Body_get_Image: Invalid BodyType");
            break;
    }
    return NULL;
}