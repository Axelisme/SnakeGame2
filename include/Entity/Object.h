#ifndef OBJECT_H
#define OBJECT_H

#include "global.h"
#include "Utility/Position.h"
#include "Utility/Direction.h"

#define TYPE_NUM 9
typedef enum OBJ_TYPE {
    O_EDGE = -1,
    O_GROUND,
    O_BODY,
    O_END,
    O_APPLE,
    O_STONE,
    O_SPIKE,
    O_BUTTON,
    O_BASIC
} OBJ_TYPE;

typedef struct Object {
    // Info
    OBJ_TYPE type;
    Pos pos;
    Direction dir;
    // Display
    ALLEGRO_BITMAP * Image;
    // method
    void (*draw)(struct Object*, ALLEGRO_BITMAP*);
    void (*deleter)(struct Object*);
} Object;

// Object
Object* new_Object(Pos pos);
void Object_init(Object* self, Pos pos);
void Object_destroy(Object* self);
void delete_Object(Object* self);
void Object_draw(Object* self, ALLEGRO_BITMAP* backbuffer);

#endif // OBJECT_H