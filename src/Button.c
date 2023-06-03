// rewrite in c
#include "Button.h"

Button* new_Button(Pos pos,ALLEGRO_BITMAP* img,EFFECT eff,Pos* eff_pos,size_t eff_len) {
    Button* button = (Button*)malloc(sizeof(Button));
    Button_init(button,pos,img,eff,eff_pos,eff_len);
    return button;
}

void Button_init(Button* button,Pos pos,ALLEGRO_BITMAP* img,EFFECT eff,Pos* eff_pos,size_t eff_len) {
    Object_init((Object*)button,pos,BUTTON,img,OBJECT_IMAGE_SIZE,OBJECT_IMAGE_SIZE);
    button->effect = eff;
    if (eff_len > 0) {
        button->Effect_pos = (Pos*)malloc(sizeof(Pos)*eff_len);
        memcpy(button->Effect_pos,eff_pos,sizeof(Pos)*eff_len);
    }
    else button->Effect_pos = NULL;
    button->Effect_len = eff_len;
    button->triger = false;
    button->obj.deleter = delete_Button;
}

void Button_destroy(Button* button) {
    Object_destroy((Object*)button);
    free(button->Effect_pos);
    button->Effect_pos = NULL;
}

void delete_Button(Object* obj) {
    Button_destroy((Button*)obj);
    free(obj);
}


/*
#include "Button.h"

Button::Button(Pos pos,ALLEGRO_BITMAP* img,EFFECT eff,std::vector<Pos> eff_pos):
        Object(pos,BUTTON,img,OBJECT_IMAGE_SIZE,OBJECT_IMAGE_SIZE) {
    effect = eff;
    Effect_pos = eff_pos;
    triger = false;
}
*/