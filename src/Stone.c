// rewrite in c

#include "Stone.h"

Stone* new_Stone(Pos pos,ALLEGRO_BITMAP * img) {
    Stone* stone = (Stone*)malloc(sizeof(Stone));
    Stone_init(stone,pos,img);
    return stone;
}
void Stone_init(Stone* self, Pos pos, ALLEGRO_BITMAP * img) {
    Object* obj = (Object*)self;
    Object_init(obj, pos, STONE, img, OBJECT_IMAGE_SIZE, OBJECT_IMAGE_SIZE);
    show_msg("Create stone begin");
    self->isFall = false;
    show_msg("Create stone done");
}

/*
#include "Stone.h"
#include <iostream>

bool Stone::update() {
    Object::update();

    if(move_dirc != NONE) {
        show_msg("Stone move begin");
        move_object(DIRC_TO_POS(move_dirc));
        move_dirc = NONE;
        show_msg("Stone move done");
    }
    return true;
}

Stone::Stone(Pos _pos,ALLEGRO_BITMAP *_img):
       Object(_pos,STONE,_img,OBJECT_IMAGE_SIZE,OBJECT_IMAGE_SIZE)
{
    show_msg("Create stone begin");
    type = STONE;
    move_dirc = NONE;

    ani_total_count = ANI_TOTAL_COUNT;
    ani_count = 0;
    ani_num = ANI_NUM;
    ani_image_idx = 0;

    show_msg("Create stone done");
}
*/