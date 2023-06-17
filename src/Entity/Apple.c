
#include "Entity/Apple.h"
#include "Engine.h"

Apple* new_Apple(ObjectVector* objs) {
    Apple* apple = (Apple*) al_calloc(1, sizeof(Apple));
    Apple_init(apple, objs);
    return apple;
}
void Apple_init(Apple* self, ObjectVector* objs) {
    show_msg("Apple_init");
    // Inherited from Entity
    Entity* Eself = (Entity*) self;
    Entity_init(Eself, objs);
    Eself->type = E_APPLE;
    Eself->isFixed = true;
    Eself->canOverlap = true;
    // method
    Eself->trigger = Apple_trigger;
}

static void Apple_trigger(Entity* Eself, MapEngine* Engine, EntityMap* Map, EntityArray* overlaps) {
    // let the snake grow
    EntityArray* activators = Eself->activators;
    Pos apple_pos = ObjV_get(&Eself->objList, 0)->pos;
    for (int i = 0; i < EA_len(activators); i++) {
        Entity* activator = EA_get(activators, i);
        if (activator->type == E_SNAKE) {
            Snake* snake = (Snake*) activator;
            Pos head_pos = snake->head->object.pos;
            if (equal(head_pos, apple_pos)) {
                Snake_grow(snake, Map, overlaps);
                Eself->Alive = false;
                break;
            }
        }
    }
}