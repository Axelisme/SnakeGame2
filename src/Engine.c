
#include "Engine.h"
#include "Entity/Snake.h"
#include "Interface/Interface.h"
#include "SoundEngine.h"

void MapEngine_init(MapEngine* self, Pos mapSize, EntityList* entities, Entity* snake) {
    show_msg("MapEngine_init");
    if (!self || !entities || !snake) {
        raise_err("MapEngine_init: init failed");
        return;
    }
    self->mapSize = mapSize;
    self->state = PLAYING;
    self->gravity = INIT_GRAVITY;
    self->entities = entities;
    self->snake = snake;
}
void MapEngine_destroy(MapEngine* self) {
    show_msg("MapEngine_destroy");
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

    // key lock
    bool keyLock = false;

    // initial mark map, overlap array, shift array
    EntityMap map; EntityMap_init(&map, self->mapSize);
    EntityArray overlaps; EntityArray_init(&overlaps);
    EntityArray shiftEs; EntityArray_init(&shiftEs);

    // mark map and get overlaps
    for (Entity* e = self->entities->front; e; e = e->next)
        e->Alive = Entity_mark(e, &map, &overlaps);
    self->snake->Alive = Entity_mark(self->snake, &map, &overlaps);

    // deal with overlaps
    _dealWithOverlap(self, &map, &overlaps);
    if (self->state != PLAYING) goto return_point;

    // check if entities are falling
    keyLock |= _canShift(self->snake, self->gravity, &map, &shiftEs);
    for (Entity* e = self->entities->front; e; e = e->next)
        _canShift(e, self->gravity, &map, &shiftEs);

    // shift all falling entities
    _ShiftEntity(&shiftEs, self->gravity, &map, &overlaps);

    // deal with overlaps
    _dealWithOverlap(self, &map, &overlaps);
    if (self->state != PLAYING) goto return_point;

    // perform operation if not key locked
    if (keyLock && op != OP_NONE) show_msg("key locked");
    if (keyLock || op == OP_NONE) goto return_point;
    Direction dir = _getDirection(op);
    if (_SnakeCanMove((Snake*)self->snake, dir, &map, &shiftEs)) {
        Entity_unmark(self->snake, &map);
        _ShiftEntity(&shiftEs, dir, &map, &overlaps);
        Snake_move((Snake*)self->snake, dir);
        self->snake->Alive = Entity_mark(self->snake, &map, &overlaps);
    }
    else {show_msg("Snake can't move"); EntityArray_clear(&shiftEs);}
    _dealWithOverlap(self, &map, &overlaps);

    // destroy map, overlap array, falling array
    return_point:
    _removeDiedEntities(self, self->entities, self->snake);
    EntityMap_destroy(&map);
    EntityArray_destroy(&overlaps);
    EntityArray_destroy(&shiftEs);

    // return state
    return self->state;
}

static void _resetAll(EntityList* entities, Entity* snake) {
    snake->reset(snake);
    for (Entity* e = entities->front; e; e = e->next)
        e->reset(e);
}
static bool _canShift(Entity* self, Direction dir, EntityMap* map, EntityArray* shiftE) {
    if (self->isFixed ||
        self->beSupported == true ||
        !self->Alive ||
        dir == DIRECTION_NONE ||
        is_empty(&self->objList)) return false;
    EntityArray localShift; EntityArray_init(&localShift);
    Entity_unmark(self, map);
    for (int i = 0; i < len(&self->objList); i++) {
        Pos pos = ObjV_get(&self->objList, i)->pos;
        Pos next_pos = neighbor_pos(pos, dir);
        Entity* next_entity = MapGet(map, next_pos);
        if (!next_entity || next_entity->canOverlap || next_entity->beSupported == false) continue;
        if (next_entity->beSupported == true || !_canShift(next_entity, dir, map, &localShift)) {
            self->beSupported = true;
            break;
        }
    }
    self->beSupported = self->beSupported == true;
    self->Alive = Entity_mark(self, map, NULL);
    if (!self->beSupported && !EntityArray_have(shiftE,self)) {
        EntityArray_push_back(shiftE, self);
        EntityArray_merge(shiftE, &localShift);
    }
    EntityArray_destroy(&localShift);
    return !self->beSupported;
}
static void _dealWithOverlap(MapEngine* self, EntityMap* map, EntityArray* overlaps) {
    for (int i = 0; i < overlaps->size; i++) {
        Entity* e = EA_get(overlaps, i);
        e->trigger(e, self, map, overlaps);
    }
    EntityArray_clear(overlaps);
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
static bool _SnakeCanMove(Snake* snake, Direction dir, EntityMap* map, EntityArray* shiftEs) {
    Entity* Esnake = (Entity*)snake;
    if (!Esnake->Alive || dir == DIRECTION_NONE) return false;
    bool canMove = true;
    Pos next_pos = Snake_next_pos(snake, dir);
    Entity* next_entity = MapGet(map, next_pos);
    if (next_entity && !next_entity->canOverlap) {
        Esnake->beSupported = true;
        canMove = _canShift(next_entity, dir, map, shiftEs);
        Esnake->beSupported = false;
    }
    return canMove;
}
static void _ShiftEntity(EntityArray* shiftEs, Direction dir, EntityMap* map, EntityArray* overlaps) {
    if (dir == DIRECTION_NONE || EA_is_empty(shiftEs)) return;
    for (int i = 0; i < shiftEs->size; i++) {
        Entity* e = EA_get(shiftEs, i);
        Entity_unmark(e, map);
        e->shift(e, dir);
    }
    for (int i = 0; i < shiftEs->size; i++) {
        Entity* e = EA_get(shiftEs, i);
        e->Alive = Entity_mark(e, map, overlaps);
    }
    EntityArray_clear(shiftEs);
}
static void _removeDiedEntities(MapEngine* self, EntityList* entities, Entity* snake) {
    for (Entity* e = entities->front; e; e = e->next) if (!e->Alive)
        EntityList_remove(entities, e);
    if (!snake->Alive) {
        SE_add_sound(died_sound, ALLEGRO_PLAYMODE_ONCE);
        self->state = LOSE;
    }
}
