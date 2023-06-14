
#include "Entity/Snake.h"
#include "Object/Body.h"

Snake* new_Snake(Object* bodies, int length) {
    Snake* snake = (Snake*) al_calloc(1, sizeof(Snake));
    Snake_init(snake, bodies, length);
    return snake;
}
void Snake_init(Snake* self, Object* bodies, int length) {
    // Inherited from Entity
    Entity* Eself = (Entity*) self;
    Entity_init(Eself);
    Eself->type = E_SNAKE;
    Eself->isFixed = false;
    Eself->canOverlap = false;
    for (int i = 0; i < length; i++)
        Eself->addObject(Eself, bodies + i);
    // Properties
    self->heading = bodies->dir;
    self->head = Eself->objList[0];
    self->tail = Eself->objList[length-1];
}
void Snake_destroy(Snake* self) {
    Entity* Eself = (Entity*) self;
    Entity_destroy(Eself);
    self->head = NULL;
    self->tail = NULL;
}
void delete_Snake(Entity* Eself) {
    Snake* self = (Snake*) Eself;
    Snake_destroy(self);
    al_free(self);
}

Pos Snake_next_pos(Snake* self, Direction dir) {
    return heading_pos(self->head->pos, dir);
}
void Snake_move(Snake* self, Direction dir) {
    Entity* Eself = (Entity*) self;
    Pos next_pos = Snake_next_pos(self, dir);
    Direction next_dir = dir;
    for (int i = 0; i < Eself->objNum; i++) {
        Object* obj = Eself->objList[i];
        Body* body = (Body*) obj;
        Pos tmp_pos = obj->pos;
        obj->pos = next_pos;
        next_pos = tmp_pos;
        Direction tmp_dir = body->from;
        body->from = body->to;
        body->to = next_dir;
        next_dir = tmp_dir;
    }
}