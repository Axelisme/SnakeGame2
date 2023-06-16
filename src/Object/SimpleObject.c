
#include "Object/SimpleObject.h"

EdgeObject* new_EdgeObject(Pos pos) {
    EdgeObject* edge = (EdgeObject*)al_calloc(1,sizeof(EdgeObject));
    EdgeObject_init(edge, pos);
    return edge;
}
void EdgeObject_init(EdgeObject* self, Pos pos){
    Object_init((Object*)self, pos);
    self->Image = edge_img;
    self->viewSize = make_Pos(1, 1);
}

AppleObject* new_AppleObject(Pos pos) {
    AppleObject* apple = (AppleObject*)al_calloc(1,sizeof(AppleObject));
    AppleObject_init(apple, pos);
    return apple;
}
void AppleObject_init(AppleObject* self, Pos pos) {
    Object_init((Object*)self, pos);
    self->Image = apple_img;
}

ButtonObject* new_ButtonObject(Pos pos) {
    ButtonObject* button = (ButtonObject*)al_calloc(1,sizeof(ButtonObject));
    ButtonObject_init(button, pos);
    return button;
}
void ButtonObject_init(ButtonObject* self, Pos pos) {
    Object_init((Object*)self, pos);
    self->Image = button_img;
}

EndObject* new_EndObject(Pos pos) {
    EndObject* end = (EndObject*)al_calloc(1,sizeof(EndObject));
    EndObject_init(end, pos);
    return end;
}
void EndObject_init(EndObject* self, Pos pos) {
    Object_init((Object*)self, pos);
    self->Image = end_img;
}

GroundObject* new_GroundObject(Pos pos) {
    GroundObject* ground = (GroundObject*)al_calloc(1,sizeof(GroundObject));
    GroundObject_init(ground, pos);
    return ground;
}
void GroundObject_init(GroundObject* self, Pos pos) {
    Object_init((Object*)self, pos);
    self->Image = ground_img;
}

SpikeObject* new_SpikeObject(Pos pos) {
    SpikeObject* spike = (SpikeObject*)al_calloc(1,sizeof(SpikeObject));
    SpikeObject_init(spike, pos);
    return spike;
}
void SpikeObject_init(SpikeObject* self, Pos pos) {
    Object_init((Object*)self, pos);
    self->Image = spike_img;
}

StoneObject* new_StoneObject(Pos pos) {
    StoneObject* stone = (StoneObject*)al_calloc(1,sizeof(StoneObject));
    StoneObject_init(stone, pos);
    return stone;
}
void StoneObject_init(StoneObject* self, Pos pos) {
    Object_init((Object*)self, pos);
    self->Image = stone_img;
}