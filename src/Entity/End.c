
#include "Entity/End.h"
#include "Engine.h"

End* new_End(Pos pos) {
    End* end = (End*) al_calloc(1, sizeof(End));
    End_init(end, pos);
    return end;
}
static void End_init(End* self, Pos pos) {
    Entity* Eself = (Entity*) self;
    ObjectVector ends; ObjectVector_init(&ends);
    EndObject end; EndObject_init(&end, pos);
    ObjV_push_back(&ends, (Object*)&end); Object_destroy((Object*)&end);
    Entity_init(Eself, &ends); ObjectVector_destroy(&ends);
    show_msg("End_init");
    // Inherited from Entity
    Eself->type = E_END;
    Eself->isFixed = true;
    Eself->canOverlap = true;
    Eself->trigger = End_trigger;
}

static void End_trigger(Entity* Eself, MapEngine* Engine, EntityMap* Map, EntityArray* overlaps) {
    // if snake head overlaps with end, then win
    EntityArray* activators = (EntityArray*) Eself->activators;
    Pos end_pos = ObjV_get(&Eself->objList, 0)->pos;
    for (int i = 0; i < activators->size; i++) {
        Entity* activator = EA_get(Eself->activators, i);
        if (activator->type == E_SNAKE) {
            Snake* snake = (Snake*) activator;
            Pos head_pos = snake->head->object.pos;
            if (equal(head_pos, end_pos)) {
                show_msg("End_trigger: win");
                Engine->state = WIN;
                break;
            }
        }
    }
}