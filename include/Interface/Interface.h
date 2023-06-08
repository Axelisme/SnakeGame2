#ifndef INTERFACCE_H
#define INTERFACCE_H

#include "global.h"

#define MAX_LIGHT 255
#define MIN_LIGHT 0
#define BASIC_LIGHT_STEP 20
#define NO_EVENT -1

typedef enum INTERFACE_TYPE {
    INTERFACE_IN_MENU,
    INTERFACE_START_MENU,
    INTERFACE_LEVEL_MENU,
    INTERFACE_GUIDE_MENU,
    INTERFACE_LEVEL,
    INTERFACE_BASIC,
    INTERFACE_NONE,
} INTERFACE_TYPE;

typedef enum INTERFACE_STATE {
    INTERFACE_INITIALING,
    INTERFACE_RUNING,
    INTERFACE_EXITING,
    INTERFACE_STOP,
    INTERFACE_DIED
} INTERFACE_STATE;

typedef struct CHILD_INFO {
    INTERFACE_TYPE next_interface;
    int level;
} CHILD_INFO;

typedef struct INTERFACE_INFO {
    INTERFACE_TYPE type;
    INTERFACE_STATE state;
    CHILD_INFO child;
} INTERFACE_INFO;

INTERFACE_INFO _default_info();
INTERFACE_INFO _fall_back_info();

typedef struct Interface {
    // Info
    INTERFACE_INFO info;
    bool should_kill;
    // Display
    int background_light;
    int background_light_up_step;
    int background_light_down_step;
    // event
    ALLEGRO_EVENT event;
    // methods
    void (*draw)(struct Interface*, ALLEGRO_BITMAP*);
    INTERFACE_INFO (*update)(struct Interface*);
    void (*event_record)(struct Interface*, ALLEGRO_EVENT);
    void (*deleter) (struct Interface*);
} Interface;

Interface* new_Interface();
void Interface_init(Interface* self);
void Interface_destroy(Interface* self);
void delete_Interface(Interface* self);
void Interface_draw(Interface* self, ALLEGRO_BITMAP* backbuffer);
INTERFACE_INFO Interface_update(Interface* self);
void Interface_event_record(Interface* self, ALLEGRO_EVENT event);
bool Interface_update_light(Interface* self, int dirc);
void draw_image(ALLEGRO_BITMAP* image, ALLEGRO_BITMAP* backbuffer);

static void _Interface_deal_event(Interface* self);

#endif // INTERFACCE_H
