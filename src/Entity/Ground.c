
#include "Entity/Ground.h"

Ground* new_Ground(GroundObject** objs, int num) {
    Ground* ground = (Ground*) al_calloc(1, sizeof(Ground));
    Ground_init(ground, objs, num);
    return ground;
}
void Ground_init(Ground* self, GroundObject** objs, int num) {
    // Inherited from Entity
    Entity* Eself = (Entity*) self;
    Entity_init(Eself, (Object**)objs, num);
    Eself->type = E_GROUND;
    Eself->isFixed = true;
    Eself->canOverlap = false;
    Eself->AliveAfterTrigger = false;
}