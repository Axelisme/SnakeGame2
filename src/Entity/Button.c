
#include "Entity/Button.h"

Entity* new_Button(ObjectVector* objs);
void Button_init(Button* self, ObjectVector* objs);
void Button_destroy(Button* self);
void delete_Button(Entity* Eself);

static void Button_setter(Entity* Eself, FILE* fp);
static void Button_trigger(Entity* Eself, MapEngine* Engine, EntityMap* Map, EntityArray* overlaps);