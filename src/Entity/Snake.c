
#include "Entity/Snake.h"
#include "Entity/EntityMap.h"

Snake* new_Snake(ObjectVector* objs) {
    Snake* snake = (Snake*) al_calloc(1, sizeof(Snake));
    Snake_init(snake, objs);
    return snake;
}
static void Snake_init(Snake* self, ObjectVector* objs) {
    show_msg("Snake_init");
    // Inherited from Entity
    Entity* Eself = (Entity*) self;
    Entity_init(Eself, objs);
    Eself->type = E_SNAKE;
    Eself->isFixed = false;
    Eself->canOverlap = false;
    Eself->draw = Snake_draw;
    // Properties
    self->head = (BodyObject*) ObjV_get(&Eself->objList, 0);
    self->tail = (BodyObject*) ObjV_get(&Eself->objList, -1);
    self->heading = self->head->to;
}
static void Snake_destroy(Snake* self) {
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

static void Snake_draw(Entity* Eself, ShiftWindow* sw, ALLEGRO_BITMAP* backbuffer) {
    Snake* self = (Snake*) Eself;
    if (!Eself->Alive)
        self->head->object.Image = dead_snake_head_img;
    Entity_draw(Eself, sw, backbuffer);
}
Pos Snake_next_pos(Snake* self, Direction dir) {
    return neighbor_pos(self->head->object.pos, dir);
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
void Snake_grow(Snake* self, EntityMap* map, EntityArray* overlays) {
    Entity* Eself = (Entity*) self;
    if (!Eself->Alive) return;
    show_msg("Snake_grow");
    BodyObject* tail = self->tail;
    Pos tail_pos = tail->object.pos;
    Direction new_tail_to = tail->from;
    Pos new_tail_pos = neighbor_pos(tail_pos, Opposite_direction(new_tail_to));
    Entity* overlay = MapGet(map, new_tail_pos);
    if(overlay && !overlay->canOverlap) {
        new_tail_to = tail->to;
        new_tail_pos = neighbor_pos(tail_pos, Opposite_direction(new_tail_to));
        overlay = MapGet(map, new_tail_pos);
        if(overlay && !overlay->canOverlap) {
            show_msg("No space! Snake can not grow.");
            return;
        }
        tail->from = new_tail_to;
    }
    Entity_unmark(Eself, map);
    tail->type = BODY;
    BodyObject new_tail; BodyObject_init(&new_tail, new_tail_pos, TAIL, new_tail_to, new_tail_to);
    ObjV_push_back(&Eself->objList, (Object*) &new_tail); BodyObject_destroy(&new_tail);
    self->tail = (BodyObject*) ObjV_get(&Eself->objList, -1);
    Entity_mark(Eself, map, overlays);
}
void Snake_shrink(Snake* self, EntityMap* map, EntityArray* overlays) {
    Entity* Eself = (Entity*) self;
    if (!Eself->Alive) return;
    show_msg("Snake_shrink");
    BodyObject* tail = self->tail;
    if (len(&Eself->objList) <= 1) {
        show_msg("No body! Snake died.");
        Eself->Alive = false;
        return;
    }
    Entity_unmark(Eself, map);
    ObjV_pop_back(&Eself->objList);
    self->tail = (BodyObject*) ObjV_get(&Eself->objList, -1);
    self->tail->type = TAIL;
    Entity_mark(Eself, map, overlays);
}