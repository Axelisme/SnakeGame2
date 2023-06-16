
#include "Object/ObjectVector.h"

ObjectVector* ObjectVector_new() {
    ObjectVector* objArray = (ObjectVector*)al_calloc(1,sizeof(ObjectVector));
    ObjectVector_init(objArray);
    return objArray;
}
void ObjectVector_init(ObjectVector* self) {
    self->size = 0;
    self->array = NULL;
}
void ObjectVector_destroy(ObjectVector* self) {
    ObjV_clear(self);
}
void delete_ObjectVector(ObjectVector* self) {
    ObjectVector_destroy(self);
    al_free(self);
}

int len(ObjectVector* self) {
    return self->size;
}
bool is_empty(ObjectVector* self) {
    return self->size == 0;
}
Object* ObjV_get(ObjectVector* self, int index) {
    index = (index + self->size) % self->size;
    return self->array[index];
}
void ObjV_push_back(ObjectVector* self, Object* object) {
    self->size++;
    self->array = (Object**)al_realloc(self->array, self->size*sizeof(Object*));
    self->array[self->size-1] = object->copy(object);
}
void ObjV_pop_back(ObjectVector* self) {
    if (self->size == 0) return;
    self->size--;
    self->array[self->size]->deleter(self->array[self->size]);
    self->array = (Object**)al_realloc(self->array, self->size*sizeof(Object*));
}
void ObjV_cat(ObjectVector* self, ObjectVector* other) {
    for (int i = 0; i < other->size; i++)
        ObjV_push_back(self, other->array[i]);
}
void ObjV_clear(ObjectVector* self) {
    for (int i = 0; i < self->size; i++)
        self->array[i]->deleter(self->array[i]);
    al_free(self->array);
    self->array = NULL;
    self->size = 0;
}