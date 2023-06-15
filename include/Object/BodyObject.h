#ifndef BODY_H
#define BODY_H

#include "Object/Object.h"

typedef enum BodyType {
    HEAD,
    BODY,
    TAIL
} BodyType;

typedef struct BodyObject {
    // Inherited from Object
    Object object;
    // Body Information
    BodyType type;
    Direction from;
    Direction to;
} BodyObject;

BodyObject* new_BodyObject(Pos pos, BodyType type, Direction from, Direction to);
void BodyObject_init(BodyObject* self, Pos pos, BodyType type, Direction from, Direction to);
void BodyObject_destroy(BodyObject* self);
void delete_BodyObject(Object* Oself);

static void BodyObject_draw(Object* Oself, ShiftWindow* sw, ALLEGRO_BITMAP* backbuffer);
static Object* BodyObject_copy(Object* self);
static ALLEGRO_BITMAP* BodyObject_get_Image(BodyObject* self, Direction* dir);

#endif // BODY_H