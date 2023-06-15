
#include "Entity/Snake.h"

Snake* new_Snake(BodyObject** bodies, int length) {
    Snake* snake = (Snake*) al_calloc(1, sizeof(Snake));
    Snake_init(snake, bodies, length);
    return snake;
}
void Snake_init(Snake* self, BodyObject** bodies, int length) {
    // Inherited from Entity
    Entity* Eself = (Entity*) self;
    Entity_init(Eself, (Object**)bodies, length);
    Eself->type = E_SNAKE;
    Eself->isFixed = false;
    Eself->canOverlap = false;
    // Properties
    self->heading = Eself->objList[0]->dir;
    self->head = (BodyObject*) Eself->objList[0];
    self->tail = (BodyObject*) Eself->objList[length-1];
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
    return heading_pos(self->head->object.pos, dir);
}
void Snake_move(Snake* self, Direction dir) {
    Entity* Eself = (Entity*) self;
    Pos next_pos = Snake_next_pos(self, dir);
    Direction next_dir = dir;
    for (int i = 0; i < Eself->objNum; i++) {
        Object* obj = Eself->objList[i];
        BodyObject* body = (BodyObject*) obj;
        Pos tmp_pos = obj->pos;
        obj->pos = next_pos;
        next_pos = tmp_pos;
        Direction tmp_dir = body->from;
        body->from = body->to;
        body->to = next_dir;
        next_dir = tmp_dir;
    }
}