
#include "Object/SimpleObject.h"

EdgeObject* new_EdgeObject(Pos pos) {
    EdgeObject* edge = new_Object(pos);
    edge->Image = edge_img;
    return edge;
}
AppleObject* new_AppleObject(Pos pos) {
    AppleObject* apple = new_Object(pos);
    apple->Image = apple_img;
    return apple;
}
ButtonObject* new_ButtonObject(Pos pos) {
    ButtonObject* button = new_Object(pos);
    button->Image = button_img;
    return button;
}
EndObject* new_EndObject(Pos pos) {
    EndObject* end = new_Object(pos);
    end->Image = end_img;
    return end;
}
GroundObject* new_GroundObject(Pos pos) {
    GroundObject* ground = new_Object(pos);
    ground->Image = ground_img;
    return ground;
}
SpikeObject* new_SpikeObject(Pos pos) {
    SpikeObject* spike = new_Object(pos);
    spike->Image = spike_img;
    return spike;
}
StoneObject* new_StoneObject(Pos pos) {
    StoneObject* stone = new_Object(pos);
    stone->Image = stone_img;
    return stone;
}