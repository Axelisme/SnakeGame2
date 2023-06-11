#ifndef OBJECT_H
#define OBJECT_H

#include "global.h"
#include "Utility/Position.h"
#include "Utility/Direction.h"

#define TYPE_NUM 10
typedef enum OBJ_TYPE {
    EDGE = -1,
    GROUND,
    BODY,
    END,
    APPLE,
    STONE,
    SPIKE,
    BUTTON
} OBJ_TYPE;

typedef struct Object {
    // Info
    OBJ_TYPE type;
    Pos pos;
    // Display
    ALLEGRO_BITMAP * Image;
    // method
    void (*draw)(struct Object*, ALLEGRO_BITMAP*);
    void (*trigger_effect)(struct Object*);
    void (*deleter)(struct Object*);
} Object;

// Object
Object* new_Object(Pos, OBJ_TYPE, ALLEGRO_BITMAP*, pos_t, pos_t);
void Object_init(Object*, Pos, OBJ_TYPE, ALLEGRO_BITMAP*, pos_t, pos_t);
void Object_destroy(Object*);
void delete_Object(Object*);
void Object_draw(Object*);
void Object_trigger_effect(Object*);

#endif // OBJECT_H