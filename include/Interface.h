#ifndef INTERFACCE_H
#define INTERFACCE_H
// rewrite in c

#include "Object.h"
#include "global.h"
#include "Position.h"

typedef struct Interface {
    // properties
    bool key_lock;
    // Display
    Pos window_center;
    pos_t window_width;
    pos_t window_height;
    pos_t width_ratio1;
    pos_t image_width1;
    pos_t image_height1;
    pos_t width_ratio2;
    pos_t image_width2;
    pos_t image_height2;
    ALLEGRO_SAMPLE *sample;
    ALLEGRO_SAMPLE_INSTANCE *backgroundSound;
    ALLEGRO_BITMAP * backgroundImage1;
    ALLEGRO_BITMAP * backgroundImage2;
    int light;
    // methods
    void (*draw)(struct Interface*);
} Interface;

Interface* new_Interface(char*, char*, char*);
void Interface_init(Interface*, char*, char*, char*);
void Interface_draw(Interface*);
bool Interface_update(Interface*);
void Interface_start_sound(Interface*);
void Interface_stop_sound(Interface*);
void Interface_go_down(Interface*);
void Interface_go_lift(Interface*);
void Interface_destroy(Interface*);
void delete_Interface(Interface*);

#endif // INTERFACCE_H

/*
#include "global.h"
#include "Object.h"
#include <string>

using namespace std;

class Interface {
public:
    bool key_lock = false;

    // draw all objects
    virtual void draw();

    // update interface
    virtual bool update();

    // start sound
    void start_sound() {al_play_sample_instance(backgroundSound);}
    // stop sound
    void stop_sound() {al_stop_sample_instance(backgroundSound);}

    void go_down();
    void go_lift();

    // load sound and backgroundImage
    Interface(string,string,string);
    void destroy_Interface();
    ~Interface();

protected:
    Pos window_center = {0,0};
    double window_width  = CHUNK_W_NUM;
    double window_height = CHUNK_H_NUM;

    double width_ratio1   = 1;

    double image_width1   = 0;
    double image_height1  = 0;

    double width_ratio2   = 1;

    double image_width2   = 0;
    double image_height2  = 0;

    ALLEGRO_SAMPLE *sample = nullptr;
    ALLEGRO_SAMPLE_INSTANCE *backgroundSound = nullptr;

    ALLEGRO_BITMAP * backgroundImage1 = nullptr;
    ALLEGRO_BITMAP * backgroundImage2 = nullptr;

    int light = 255;
};
*/

