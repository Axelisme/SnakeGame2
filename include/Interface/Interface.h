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

typedef struct Interface {
    // Info
    INTERFACE_TYPE type;
    // Display
    int background_light;
    int background_light_up_step;
    int background_light_down_step;
    // state
    INTERFACE_STATE state;
    bool should_kill;
    INTERFACE_TYPE next_interface;
    // event
    ALLEGRO_EVENT event;
    // methods
    void (*draw)(struct Interface*, ALLEGRO_BITMAP*);
    INTERFACE_STATE (*update)(struct Interface*);
    void (*event_record)(struct Interface*, ALLEGRO_EVENT);
    void (*deleter) (struct Interface*);
} Interface;

Interface* new_Interface();
void Interface_init(Interface* self);
void Interface_destroy(Interface* self);
void delete_Interface(Interface* self);
void Interface_draw(Interface* self, ALLEGRO_BITMAP* backbuffer);
INTERFACE_STATE Interface_update(Interface* self);
void Interface_event_record(Interface* self, ALLEGRO_EVENT event);

bool _Interface_update_light(Interface* self, int dirc);
static void _Interface_deal_event(Interface* self);
void _draw_image(ALLEGRO_BITMAP* image, ALLEGRO_BITMAP* backbuffer);

#endif // INTERFACCE_H
