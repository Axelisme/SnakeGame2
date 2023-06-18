#ifndef OBJECT_H
#define OBJECT_H

#include "global.h"
#include "Utility/Position.h"
#include "Utility/Direction.h"
#include "Utility/ShiftWindow.h"

// Image
extern ALLEGRO_BITMAP* edge_img;
extern ALLEGRO_BITMAP* apple_img;
extern ALLEGRO_BITMAP* button_img;
extern ALLEGRO_BITMAP* end_img;
extern ALLEGRO_BITMAP* ground_img;
extern ALLEGRO_BITMAP* snake_head_img;
extern ALLEGRO_BITMAP* dead_snake_head_img;
extern ALLEGRO_BITMAP* snake_body_straight_img;
extern ALLEGRO_BITMAP* snake_body_turn_img;
extern ALLEGRO_BITMAP* snake_tail_img;
extern ALLEGRO_BITMAP* spike_img;
extern ALLEGRO_BITMAP* stone_img;
extern ALLEGRO_BITMAP* box_img;

typedef struct Object {
    // Info
    Pos pos;
    Direction dir;
    // Display
    ALLEGRO_BITMAP * Image;
    Pos viewSize;
    // method
    void (*draw)(struct Object*, ShiftWindow*, ALLEGRO_BITMAP*);
    void (*shift)(struct Object*, Pos);
    struct Object* (*copy)(struct Object*);
    void (*deleter)(struct Object*);
} Object;

// ObjectClass
void ObjectClass_init();
void ObjectClass_destroy();

// Object
Object* new_Object(Pos pos);
void Object_init(Object* self, Pos pos);
void Object_destroy(Object* self);
void delete_Object(Object* self);
Object* Object_copy(Object* self);
void Object_draw(Object* self, ShiftWindow* sw, ALLEGRO_BITMAP* backbuffer);
void Object_shift(Object* self, Pos delta);

#endif // OBJECT_H