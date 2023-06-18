
#include "Entity/Stone.h"

Stone* new_Stone(Pos pos) {
    Stone* stone = (Stone*)al_calloc(1,sizeof(Stone));
    Stone_init(stone, pos);
    return stone;
}
void Stone_init(Stone* self, Pos pos) {
    Entity* Eself = (Entity*)self;
    ObjectVector stones; ObjectVector_init(&stones);
    StoneObject stone; StoneObject_init(&stone, pos);
    ObjV_push_back(&stones, (Object*)&stone); Object_destroy((Object*)&stone);
    Entity_init(Eself, &stones); ObjectVector_destroy(&stones);
    show_msg("Stone_init");
    // Inherited from Entity
    Eself->type = E_STONE;
    Eself->isFixed = false;
    Eself->canOverlap = false;
}