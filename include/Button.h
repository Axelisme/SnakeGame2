#ifndef BUTTON_H
#define BUTTON_H

#include "global.h"
#include "Object.h"

typedef struct Button {
    Object obj;
    EFFECT effect;
    Pos* Effect_pos;
    size_t Effect_len;
    bool triger;
} Button;

Button* new_Button(Pos,ALLEGRO_BITMAP*,EFFECT,Pos*,size_t);
void Button_init(Button*,Pos,ALLEGRO_BITMAP*,EFFECT,Pos*,size_t);
void Button_destroy(Button*);
void delete_Button(Object*);

#endif // BUTTON_H
/*
#include "global.h"
#include "Object.h"
#include <vector>

class Button: public Object {
public:
    Button(Pos,ALLEGRO_BITMAP*,EFFECT,std::vector<Pos>);
};
*/
