#ifndef INTERFACE_H
#define INTERFACE_H

#include "global.h"
#include "Utility/Direction.h"

#define MAX_LIGHT 255
#define MIN_LIGHT 0
#define BASIC_LIGHT_STEP 20
#define NO_EVENT -1

typedef enum INTERFACE_TYPE {
    INTERFACE_IN_MENU,
    INTERFACE_START_MENU,
    INTERFACE_GUIDE_MENU,
    INTERFACE_LEVEL_MENU,
    INTERFACE_LEVEL_START,
    INTERFACE_LEVEL,
    INTERFACE_BASIC,
    INTERFACE_SINGLE,
    INTERFACE_MULTI,
    INTERFACE_NONE,
} INTERFACE_TYPE;

typedef enum INTERFACE_STATE {
    INTERFACE_INITIALING,
    INTERFACE_RUNNING,
    INTERFACE_EXITING,
    INTERFACE_STOP,
    INTERFACE_DIED
} INTERFACE_STATE;

typedef struct CHILD_INFO {
    INTERFACE_TYPE interface_type;
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
    int background_light_max;
    int background_light_min;
    int background_light_up_step;
    int background_light_down_step;
    // event
    ALLEGRO_EVENT event;
    // methods
    void (*draw)(struct Interface*, ALLEGRO_BITMAP*);
    INTERFACE_INFO (*update)(struct Interface*);
    void (*event_record)(struct Interface*, ALLEGRO_EVENT);
    void (*event_dealer)(struct Interface*);
    void (*deleter) (struct Interface*);
} Interface;

Interface* new_Interface();
void Interface_init(Interface* self);
void Interface_destroy(Interface* self);
void delete_Interface(Interface* self);
void Interface_draw(Interface* self, ALLEGRO_BITMAP* backbuffer);
INTERFACE_INFO Interface_update(Interface* self);
void Interface_event_record(Interface* self, ALLEGRO_EVENT event);
void Interface_deal_event(Interface* self);
void Interface_set_kill(Interface* self, INTERFACE_TYPE next_type);
void Interface_set_stop(Interface* self, INTERFACE_TYPE next_type);
bool Interface_light_up(Interface* self);
bool Interface_light_down(Interface* self);
void draw_image(ALLEGRO_BITMAP* image, ALLEGRO_BITMAP* backbuffer, Direction direction);


#endif // INTERFACE_H
