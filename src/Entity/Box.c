
#include "Entity/Box.h"

Box* new_Box(ObjectVector* objs) {
    Box* box = (Box*)al_calloc(1,sizeof(Box));
    Box_init(box, objs);
    return box;
}
void Box_init(Box* self, ObjectVector* objs) {
    Entity* Eself = (Entity*)self;
    Entity_init(Eself, objs);
    show_msg("Box_init");
    // Inherited from Entity
    Eself->type = E_BOX;
    Eself->isFixed = false;
    Eself->canOverlap = false;
}