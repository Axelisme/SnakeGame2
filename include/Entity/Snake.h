#ifndef SNAKE_H
#define SNAKE_H

#include "Entity/Entity.h"

typedef struct Snake {
    // inherited from Entity
    Entity entity;
    // Properties
    Direction heading;
    Object* head;
    Object* tail;
} Snake;

Snake* new_Snake(Object* bodies, int length);
void Snake_init(Snake* self, Object* bodies, int length);
void Snake_destroy(Snake* self);
void delete_Snake(Entity* Eself);

Pos Snake_next_pos(Snake* self, Direction dir);
void Snake_move(Snake* self, Direction dir);


#endif // SNAKE_H
