// rewrite in c
#include "Object.h"
#include "global.h"

Object* newObject(Pos _pos, OBJ_TYPE T, ALLEGRO_BITMAP* _img, pos_t _w, pos_t _h) {
    Object* obj = (Object*)malloc(sizeof(Object));
    Object_init(obj,_pos,T,_img,_w,_h);
    return obj;
}

void Object_init(Object* obj, Pos _pos, OBJ_TYPE T, ALLEGRO_BITMAP* _img, pos_t _w, pos_t _h) {
    // base Info
    obj->type = T;
    obj->pos = _pos;
    obj->move_dirc = NONE;
    // Display
    obj->width = _w;
    obj->height = _h;
    obj->Image = _img;
    obj->image_width = al_get_bitmap_width(obj->Image)/CHUNK_WIDTH;
    obj->image_height = al_get_bitmap_height(obj->Image)/CHUNK_HEIGHT;
    obj->rotate_angle = 0;
    obj->window_x = 0;
    obj->window_y = 0;
    // animation
    obj->ani_total_count = ANI_TOTAL_COUNT;
    obj->ani_count = 0;
    obj->ani_num = ANI_NUM;
    obj->ani_image_idx = 0;
    // method
    obj->draw = Object_draw;
    obj->update = Object_update;
    obj->move = Object_move;
    obj->deleter = delete_Object;
}

void Object_draw(Object* obj) {
    if(obj->Image==NULL) {
        raise_warn("try to draw null object");
        return;
    }

    al_draw_scaled_bitmap(obj->Image,
                            0,0,
                            al_get_bitmap_width(obj->Image),al_get_bitmap_height(obj->Image),
                            CHUNK_WIDTH *(obj->pos.x - obj->window_x - OBJECT_IMAGE_SIZE/2),
                            CHUNK_HEIGHT*(obj->pos.y  - obj->window_y - OBJECT_IMAGE_SIZE/2),
                            OBJECT_IMAGE_SIZE*CHUNK_WIDTH , OBJECT_IMAGE_SIZE*CHUNK_HEIGHT,
                            0
                            );
}

bool Object_update(Object* obj) {
    if((++obj->ani_count)%obj->ani_total_count == 0)
        obj->ani_image_idx = (++obj->ani_image_idx)%obj->ani_num;
    if(obj->move_dirc != NONE) {
        show_msg("Object move begin");
        Object_move(obj, obj->move_dirc);
        obj->move_dirc = NONE;
        show_msg("Object move done");
    }
    return true;
}

void Object_move(Object* obj, DIRCTION D) {
    const Pos dp = DIRC_TO_POS(D);
    obj->pos = add(obj->pos,dp);
}

void Object_destroy(Object* obj) {
    return;
}

void delete_Object(Object* obj) {
    Object_destroy(obj);
    free(obj);
}


/*
Pos Object::ConvertIdx(int idx) {
    switch(idx) {
        case 0: return std::make_pair(0.0,0.0);
        case 1: return std::make_pair(0.0,0.5);
        case 2: return std::make_pair(0.5,0.0);
        case 3: return std::make_pair(0.5,0.5);
        default: {
            raise_warn("want to convert invalid idx");
            return {0.0,0.0};
        }
    }
}

void Object::draw() {
    if(Image==nullptr) {
        raise_warn("try to draw null object"); 
        return;
    }

    al_draw_scaled_bitmap(Image,
                            0,0,
                            al_get_bitmap_width(Image),al_get_bitmap_height(Image),
                            CHUNK_WIDTH *(pos.second - window_x - OBJECT_IMAGE_SIZE/2),
                            CHUNK_HEIGHT*(pos.first  - window_y - OBJECT_IMAGE_SIZE/2),
                            OBJECT_IMAGE_SIZE*CHUNK_WIDTH , OBJECT_IMAGE_SIZE*CHUNK_HEIGHT,
                            0
                            );
}

bool Object::update() {
    if((++ani_count)%ani_total_count == 0) {
        ani_image_idx = (++ani_image_idx)%ani_num;
        return true;
    }
    return false;
}

Object::Object(Pos _pos,OBJ_TYPE T,ALLEGRO_BITMAP* _img,float _w,float _h) {
    pos = _pos;
    type = T;

    image_width = 0;
    image_height = 0;

    rotate_angle = 0;

    Image = _img;
    if(Image!=nullptr) {
        image_width = al_get_bitmap_width(Image)/CHUNK_WIDTH;
        image_height = al_get_bitmap_height(Image)/CHUNK_HEIGHT;
    }
    else raise_warn("load object image fail");

    width = _w;
    height = _h;
}
*/