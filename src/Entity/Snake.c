
#include "Entity/Snake.h"

Snake* new_Snake(ObjectVector* objs) {
    Snake* snake = (Snake*) al_calloc(1, sizeof(Snake));
    Snake_init(snake, objs);
    return snake;
}
void Snake_init(Snake* self, ObjectVector* objs) {
    show_msg("Snake_init");
    // Inherited from Entity
    Entity* Eself = (Entity*) self;
    Entity_init(Eself, objs);
    Eself->type = E_SNAKE;
    Eself->isFixed = false;
    Eself->canOverlap = false;
    // Properties
    self->head = (BodyObject*) ObjV_get(&Eself->objList, 0);
    self->tail = (BodyObject*) ObjV_get(&Eself->objList, -1);
    self->heading = self->head->to;
}
void Snake_destroy(Snake* self) {
    show_msg("Snake_destroy");
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
    show_msg("Snake_move");
    Entity* Eself = (Entity*) self;
    Pos next_pos = Snake_next_pos(self, dir);
    Direction next_to = dir;
    Direction next_from = dir;
    self->head->to = dir;
    for (int i = 0; i < len(&Eself->objList); i++) {
        Object* obj = ObjV_get(&Eself->objList, i);
        BodyObject* body = (BodyObject*) obj;
        swap(next_pos, obj->pos);
        swap(next_to, body->to);
        swap(next_from, body->from);
    }
}