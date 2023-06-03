//rewrite in c
#include "Interface.h"

Interface* new_Interface(char* sound_path, char* back1path, char* back2path) {
    Interface* interface = (Interface*)malloc(sizeof(Interface));
    Interface_init(interface, sound_path, back1path, back2path);
    return interface;
}
void Interface_init(Interface* infer, char* sound_path, char* back1path, char* back2path) {
    show_msg("Create Interface begin");
    // init properties
    infer->key_lock = false;
    infer->window_center = makePos(0,0);
    infer->window_width = CHUNK_W_NUM;
    infer->window_height = CHUNK_H_NUM;
    infer->width_ratio1 = 1;
    infer->image_width1 = 0;
    infer->image_height1 = 0;
    infer->width_ratio2 = 1;
    infer->image_width2 = 0;
    infer->image_height2 = 0;
    infer->light = 255;
    infer->sample = NULL;
    infer->backgroundSound = NULL;
    infer->backgroundImage1 = NULL;
    infer->backgroundImage2 = NULL;

    // load sound
    infer->sample = al_load_sample(sound_path);
    if (infer->sample == NULL)
        raise_warn("Can't load sound");
    else {
        infer->backgroundSound = al_create_sample_instance(infer->sample);
        al_set_sample_instance_playmode(infer->backgroundSound, ALLEGRO_PLAYMODE_LOOP);
        al_attach_sample_instance_to_mixer(infer->backgroundSound, al_get_default_mixer());
    }


    // load background1 image
    infer->backgroundImage1 = al_load_bitmap(back1path);
    if (infer->backgroundImage1 == NULL)
        raise_warn("Can't load background image");
    else {
        infer->image_width1 =  al_get_bitmap_width(infer->backgroundImage1)/CHUNK_WIDTH;
        infer->image_height1 = al_get_bitmap_height(infer->backgroundImage1)/CHUNK_HEIGHT;
        infer->width_ratio1 = (infer->image_width1-infer->window_width)
                                /(SEE_MAP_RIGHT-SEE_MAP_LEFT - infer->window_width);
    }

    // load background2 image
    infer->backgroundImage2 = al_load_bitmap(back2path);
    if (infer->backgroundImage2 == NULL)
        raise_warn("Can't load background image");
    else {
        infer->image_width2 =  al_get_bitmap_width(infer->backgroundImage2)/CHUNK_WIDTH;
        infer->image_height2 = al_get_bitmap_height(infer->backgroundImage2)/CHUNK_HEIGHT;
        infer->width_ratio2 = (infer->image_width2-infer->window_width)
                                /(SEE_MAP_RIGHT-SEE_MAP_LEFT - infer->window_width);
    }

    // method
    infer->draw = Interface_draw;

    show_msg("Create interface done");
}
void Interface_draw(Interface* infer) {
    al_clear_to_color(WRITE);
    float sx = CHUNK_WIDTH*infer->width_ratio1*(infer->window_center.x-infer->window_width/2-SEE_MAP_LEFT);
    float sy = 0;
    float sw = CHUNK_WIDTH*(infer->window_width);
    float sh = al_get_bitmap_height(infer->backgroundImage1);
    float dx = 0;
    float dy = 0;
    float dw = CHUNK_WIDTH*infer->window_width;
    float dh = CHUNK_HEIGHT*infer->window_height;
    if(SHOW_BACK_FOG) al_draw_scaled_bitmap(infer->backgroundImage1,sx,sy,sw,sh,dx,dy,dw,dh,0);
    sx = CHUNK_WIDTH*infer->width_ratio2*(infer->window_center.x-infer->window_width/2-SEE_MAP_LEFT);
    sy = 0;
    sw = CHUNK_WIDTH*(infer->window_width);
    sh = al_get_bitmap_height(infer->backgroundImage2);
    dx = 0;
    dy = 0;
    dw = CHUNK_WIDTH*infer->window_width;
    dh = CHUNK_HEIGHT*infer->window_height;
    al_draw_scaled_bitmap(infer->backgroundImage2,sx,sy,sw,sh,dx,dy,dw,dh,0);
}
bool Interface_update(Interface* infer){return false;}
void Interface_start_sound(Interface* infer){
    if (infer == NULL) raise_err("Interface is NULL");
    else if (infer->backgroundSound == NULL) raise_warn("Can't play sound");
    else al_play_sample_instance(infer->backgroundSound);
}
void Interface_stop_sound(Interface* infer){
    if (infer == NULL) raise_err("Interface is NULL");
    else if (infer->backgroundSound == NULL) raise_warn("Can't stop sound");
    else al_stop_sample_instance(infer->backgroundSound);
}
void Interface_go_down(Interface* infer){
    show_msg("show down animation begin");
    infer->key_lock = true;
    int lock = true;
    while(lock){
        infer->light -= 2;
        if(infer->light<=0) {
            infer->light = 0;
            lock = false;
        }
    }
    infer->key_lock = false;
    show_msg("show down animation done");
}
void Interface_go_lift(Interface* infer){
    show_msg("show lift animation begin");
    infer->key_lock = true;
    int lock = true;
    while(lock){
        infer->light += 2;
        if(infer->light>=255) {
            infer->light = 255;
            lock = false;
        }
        infer->draw(infer);
    }
    infer->key_lock = false;
    show_msg("show lift animation done");
}
void Interface_destroy(Interface* infer){
    if (infer == NULL) return;
    show_msg("destroy interface begin");
    al_destroy_sample_instance(infer->backgroundSound);
    al_destroy_sample(infer->sample);
    al_destroy_bitmap(infer->backgroundImage1);
    al_destroy_bitmap(infer->backgroundImage2);
    infer->backgroundSound = NULL;
    infer->sample = NULL;
    infer->backgroundImage1 = NULL;
    infer->backgroundImage2 = NULL;
    show_msg("destroy interface done");
}
void delete_Interface(Interface* infer){
    Interface_destroy(infer);
    free(infer);
}
/*
#include "Interface.h"
#include "GameWindow.h"

void Interface::draw() {
    al_clear_to_color(WRITE);
    float sx = CHUNK_WIDTH*width_ratio1*(window_center.second-window_width/2-SEE_MAP_LEFT);
    float sy = 0;
    float sw = CHUNK_WIDTH*(window_width);
    float sh = al_get_bitmap_height(backgroundImage1);
    float dx = 0;
    float dy = 0;
    float dw = CHUNK_WIDTH*window_width;
    float dh = CHUNK_HEIGHT*window_height;
    if(SHOW_BACK_FOG) al_draw_scaled_bitmap(backgroundImage1,sx,sy,sw,sh,dx,dy,dw,dh,0);
    sx = CHUNK_WIDTH*width_ratio2*(window_center.second-window_width/2-SEE_MAP_LEFT);
    sy = 0;
    sw = CHUNK_WIDTH*(window_width);
    sh = al_get_bitmap_height(backgroundImage2);
    dx = 0;
    dy = 0;
    dw = CHUNK_WIDTH*window_width;
    dh = CHUNK_HEIGHT*window_height;
    al_draw_scaled_bitmap(backgroundImage2,sx,sy,sw,sh,dx,dy,dw,dh,0);
}

bool Interface::update() {
    return false;
}

Interface::Interface(string sound_path="",string back1path="",string back2path="") {
    show_msg("Create Interface begin");
    // load sound
    if(!sound_path.empty()) {
        sample = al_load_sample(sound_path.c_str());
        if(sample==nullptr) backgroundSound = nullptr;
        else {
            backgroundSound = al_create_sample_instance(sample);
            al_set_sample_instance_playmode(backgroundSound, ALLEGRO_PLAYMODE_LOOP);
            al_attach_sample_instance_to_mixer(backgroundSound, al_get_default_mixer());
        }
    }
    else raise_warn("no sound path given");

    window_width = CHUNK_W_NUM;
    window_height = CHUNK_H_NUM;
    width_ratio1 = 1;
    image_width1 = 0;
    image_height1 = 0; 
    width_ratio2 = 1;
    image_width2 = 0;
    image_height2 = 0; 

    // load background1 image
    if(!back1path.empty()) {
        backgroundImage1 = al_load_bitmap(back1path.c_str());
        if(backgroundImage1==nullptr) {
            raise_err("Can't not load background image because invalid path");
        }
        image_width1 =  al_get_bitmap_width(backgroundImage1)/CHUNK_WIDTH;
        image_height1 = al_get_bitmap_height(backgroundImage1)/CHUNK_HEIGHT;
        width_ratio1 = (image_width1-window_width)/(SEE_MAP_RIGHT-SEE_MAP_LEFT-window_width);
    }
    else raise_warn("no image path given for background 1");

    // load background2 image
    if(!back2path.empty()) {
        backgroundImage2 = al_load_bitmap(back2path.c_str());
        if(backgroundImage2==nullptr) {
            raise_err("Can't not load background image because invalid path");
        }
        image_width2 =  al_get_bitmap_width(backgroundImage2)/CHUNK_WIDTH;
        image_height2 = al_get_bitmap_height(backgroundImage2)/CHUNK_HEIGHT;
        width_ratio2 = (image_width2-window_width)/(SEE_MAP_RIGHT-SEE_MAP_LEFT-window_width);
    }
    else raise_warn("no image path given for background 2");


    show_msg("Create interface done");
}

void Interface::go_down() {
    show_msg("show down animation begin");
    key_lock = true;
    int lock = true;
    while(lock){
        light -= 2;
        if(light<=0) {
            light = 0;
            lock = false;
        }
    }
    key_lock = false;
    show_msg("show down animation done");
}

void Interface::go_lift() {
    show_msg("show lift animation begin");
    key_lock = true;
    int lock = true;
    while(lock){
        light += 2;
        if(light>=255) {
            light = 255;
            lock = false;
        }
        draw();
    }
    key_lock = false;
    show_msg("show lift animation done");
}

void Interface::destroy_Interface() {
    show_msg("Destroy interface begin");
    al_destroy_sample_instance(backgroundSound);
    al_destroy_sample(sample);
    al_destroy_bitmap(backgroundImage1);
    al_destroy_bitmap(backgroundImage2);
    backgroundSound = nullptr;
    sample = nullptr;
    backgroundImage1 = nullptr;
    backgroundImage2 = nullptr;

    show_msg("Destroy interface done");
}

Interface::~Interface() {
    show_msg("Delete interface begin");
    destroy_Interface();
    show_msg("Delete interface done");
}
*/