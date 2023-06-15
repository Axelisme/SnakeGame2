#ifndef SNAKE_H
#define SNAKE_H

#include "Entity/Entity.h"
#include "Object/BodyObject.h"

typedef struct Snake {
    // inherited from Entity
    Entity entity;
    // Properties
    Direction heading;
    BodyObject* head;
    BodyObject* tail;
} Snake;

Snake* new_Snake(BodyObject** bodies, int length);
void Snake_init(Snake* self, BodyObject** bodies, int length);
void Snake_destroy(Snake* self);
void delete_Snake(Entity* Eself);

Pos Snake_next_pos(Snake* self, Direction dir);
void Snake_move(Snake* self, Direction dir);


#endif // SNAKE_H
