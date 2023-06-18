#ifndef BUTTON_H
#define BUTTON_H

#include "Entity.h"

typedef enum BUTTON_EFFECT {
    SNAKE_SHRINK,
    GRAVITY_REVERSE,
    SPIKE_SET,
} BUTTON_EFFECT;

typedef struct SnakeShrinkInfo {
    int shrink_num;
} SnakeShrinkInfo;

typedef struct GravityReverseInfo {
    Direction gravity;
} GravityReverseInfo;

typedef struct SpikeSetInfo {
    int spike_num;
    Pos* spike_pos;
} SpikeSetInfo;

typedef union EFFECT_INFO {
    SnakeShrinkInfo    snakeS;
    GravityReverseInfo gravityR;
    SpikeSetInfo       spikeS;
} EFFECT_INFO;

typedef struct Button {
    // Inherited from Entity
    Entity entity;
    // state
    bool isPressed;
    // properties
    BUTTON_EFFECT effect;
    EFFECT_INFO effect_info;
} Button;

Entity* new_Button(ObjectVector* objs);
void Button_init(Button* self, ObjectVector* objs);
void Button_destroy(Button* self);
void delete_Button(Entity* Eself);

static void Button_setter(Entity* Eself, FILE* fp);
static void Button_trigger(Entity* Eself, MapEngine* Engine, EntityMap* Map, EntityArray* overlaps);

#endif // !BUTTON_H