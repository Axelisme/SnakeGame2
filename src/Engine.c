
#include "Engine.h"
#include "Entity/Snake.h"

void MapEngine_init(MapEngine* self, Pos mapSize, EntityList* entities, Entity* snake) {
    show_msg("MapEngine_init");
    self->mapSize = mapSize;
    self->state = PLAYING;
    self->entities = entities;
    self->snake = snake;
}
void MapEngine_destroy(MapEngine* self) {
    show_msg("MapEngine_destroy");
    if (self->entities) EntityList_destroy(self->entities);
    if (self->snake) self->snake->deleter(self->snake);
}
void MapEngine_draw(MapEngine* self, ShiftWindow* sw, ALLEGRO_BITMAP* backbuffer) {
    if (!self->entities) return;
    for (Entity* e = self->entities->front; e; e = e->next)
        e->draw(e, sw, backbuffer);
    if (self->snake)
        self->snake->draw(self->snake, sw, backbuffer);
}
PLAYER_STATE MapEngine_process(MapEngine* self, OPERATION op) {
    // check if game is over
    if (self->state != PLAYING) return self->state;

    // Died if entities is null
    if (!self->snake) {raise_warn("no snake exist"); self->state = LOSE;return self->state;}

    // key lock
    bool keyLock = false;

    // initial mark map, overlap array, shift array
    EntityMap map; EntityMap_init(&map, self->mapSize);
    EntityArray overlaps; EntityArray_init(&overlaps);
    EntityArray shiftObjs; EntityArray_init(&shiftObjs);

    // mark map and get overlaps
    if (self->entities) for (Entity* e = self->entities->front; e; e = e->next)
        Entity_mark(e, &map, &overlaps);
    Entity_mark(self->snake, &map, &overlaps);

    // deal with overlaps
    _dealWithOverlap(self, &map, &overlaps);
    if (self->state != PLAYING) goto return_point;

    // check if entities are falling
    keyLock |= _canShift(self->snake, map.gravity, &map, &shiftObjs);
    if (self->entities) for (Entity* e = self->entities->front; e; e = e->next)
        _canShift(e, map.gravity, &map, &shiftObjs);

    // shift all falling entities
    _ShiftEntity(&shiftObjs, map.gravity, &map, &overlaps);

    // deal with overlaps
    _dealWithOverlap(self, &map, &overlaps);
    if (self->state != PLAYING) goto return_point;

    // perform operation if not key locked
    if (keyLock || op == OP_NONE) goto return_point;
    Direction dir = _getDirection(op);
    if (_SnakeCanMove((Snake*)self->snake, dir, &map, &shiftObjs)) {
        Entity_unmark(self->snake, &map);
        _ShiftEntity(&shiftObjs, dir, &map, &overlaps);
        Snake_move((Snake*)self->snake, dir);
        Entity_mark(self->snake, &map, &overlaps);
    }
    else EntityArray_clear(&shiftObjs);
    _dealWithOverlap(self, &map, &overlaps);

    // destroy map, overlap array, falling array
    return_point:
    EntityMap_destroy(&map);
    EntityArray_destroy(&overlaps);
    EntityArray_destroy(&shiftObjs);

    // return state
    return self->state;
}

static void _resetAll(EntityList* entities, Entity* snake) {
    snake->reset(snake);
    if (entities) for (Entity* e = entities->front; e; e = e->next)
        e->reset(e);
}
static bool _canShift(Entity* self, Direction dir, EntityMap* map, EntityArray* shiftObjs) {
    if (!self->objList) return false;
    if (self->isFixed || self->beSupported) return false;
    Entity_unmark(self, map);
    for (int i = 0; i < self->objNum; i++) {
        Pos pos = self->objList[i]->pos;
        Pos next_pos = heading_pos(pos, dir);
        Entity* next_entity = MapGet(map, next_pos);
        if (!next_entity || next_entity->canOverlap) continue;
        if (!_canShift(next_entity, dir, map, shiftObjs)) {
            self->beSupported = true;
            break;
        }
    }
    Entity_mark(self, map, NULL);
    if (!self->beSupported && !EntityArray_have(shiftObjs,self)) 
        EntityArray_push_back(shiftObjs, self);
    return !self->beSupported;
}
static void _dealWithOverlap(MapEngine* self, EntityMap* map, EntityArray* overlaps) {
    for (int i = 0; i < overlaps->size; i++) {
        Entity* e = overlaps->array[i];
        e->trigger(e, map, overlaps);
        if (e->TriggerTimes <= 0) {
            EntityList_remove(self->entities, e);
            e->deleter(e);
        }
    }
    EntityArray_clear(overlaps);
    self->state = map->levelState;
    _resetAll(self->entities, self->snake);
}
static Direction _getDirection(OPERATION op) {
    switch (op) {
        case OP_MOVE_UP: return DIRECTION_UP;
        case OP_MOVE_DOWN: return DIRECTION_DOWN;
        case OP_MOVE_LEFT: return DIRECTION_LEFT;
        case OP_MOVE_RIGHT: return DIRECTION_RIGHT;
        default:
            raise_warn("Invalid operation");
            return DIRECTION_NONE;
    }
}
static bool _SnakeCanMove(Snake* snake, Direction dir, EntityMap* map, EntityArray* shiftObjs) {
    Entity* Esnake = (Entity*)snake;
    bool canMove = true;
    Pos next_pos = Snake_next_pos(snake, dir);
    Entity* next_entity = MapGet(map, next_pos);
    if (next_entity && !next_entity->canOverlap)
        canMove = _canShift(next_entity, dir, map, shiftObjs);
    return canMove;
}
static void _ShiftEntity(EntityArray* shiftObjs, Direction dir, EntityMap* map, EntityArray* overlaps) {
    for (int i = 0; i < shiftObjs->size; i++) {
        Entity* e = shiftObjs->array[i];
        Entity_unmark(e, map);
        e->shift(e, map->gravity);
    }
    for (int i = 0; i < shiftObjs->size; i++)
        Entity_mark(shiftObjs->array[i], map, overlaps);
    EntityArray_clear(shiftObjs);
}