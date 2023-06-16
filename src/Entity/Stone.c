
#include "Entity/Stone.h"

Stone* new_Stone(ObjectVector* objs) {
    Stone* stone = (Stone*)al_calloc(1,sizeof(Stone));
    Stone_init(stone, objs);
    return stone;
}
void Stone_init(Stone* self, ObjectVector* objs) {
    Entity* Eself = (Entity*)self;
    Entity_init(Eself, objs);
    show_msg("Stone_init");
    // Inherited from Entity
    Eself->type = E_STONE;
    Eself->isFixed = false;
    Eself->canOverlap = false;
}