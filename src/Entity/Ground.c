
#include "Entity/Ground.h"

Ground* new_Ground(ObjectVector* objs) {
    Ground* ground = (Ground*) al_calloc(1, sizeof(Ground));
    Ground_init(ground, objs);
    return ground;
}
void Ground_init(Ground* self,ObjectVector* objs) {
    // Inherited from Entity
    Entity* Eself = (Entity*) self;
    Entity_init(Eself, objs);
    Eself->type = E_GROUND;
    Eself->isFixed = true;
    Eself->canOverlap = false;
    Eself->AliveAfterTrigger = false;
}