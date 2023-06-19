#ifndef BUTTON_H
#define BUTTON_H

#include "Entity.h"
#define EFFECT_LINE_MAX 50
#define BUTTON_MAX_SPIKE_NUM 10

typedef enum BUTTON_EFFECT {
    SNAKE_SHRINK    = 0,
    GRAVITY_CHANGE  = 1,
    SPIKE_SET       = 2,
} BUTTON_EFFECT;

typedef struct SnakeShrinkInfo {
    int shrink_num;
} SnakeShrinkInfo;

typedef struct GravityReverseInfo {
    Direction gravity;
} GravityReverseInfo;

typedef struct SpikeSetInfo {
    int spike_num;
    Pos spike_pos[BUTTON_MAX_SPIKE_NUM];
    Direction spike_dir[BUTTON_MAX_SPIKE_NUM];
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
    int effect_remain;
    // properties
    BUTTON_EFFECT effect;
    int effect_repeat;
    EFFECT_INFO effect_info;
} Button;

Entity* new_Button(ObjectVector* objs);
void Button_init(Button* self, ObjectVector* objs);

static void Button_setter(Entity* Eself, FILE* fp);
static void Button_trigger(Entity* Eself, MapEngine* Engine, EntityMap* Map, EntityArray* overlaps);

#endif // !BUTTON_H