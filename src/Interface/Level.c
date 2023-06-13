
#include "Interface/Level.h"

Level* new_Level(LEVEL_ID level_id) {
    Level* level = (Level*)al_calloc(1,sizeof(Level));
    Level_init(level, level_id);
    return level;
}
void Level_init(Level* self, LEVEL_ID level_id) {
    if (self == nullptr) {raise_warn("Level_init: self is nullptr"); return;}
    Interface* Iself = (Interface*)self;
    Interface_init(Iself);
    show_msg("Level_init");
    // Inherited from Interface
    Iself->info.type = INTERFACE_LEVEL;
    Iself->draw = Level_draw;
    Iself->event_record = Level_event_recorder;
    Iself->event_dealer = Level_event_dealer;
    Iself->deleter = delete_Level;
    // Info
    self->level_id = level_id;
    // Load level
    Level_loader(self, level_id);
}
void Level_destroy(Level* self) {
    if (self == nullptr) {raise_warn("Level_destroy: self is nullptr"); return;}
    show_msg("Level_destroy");
    // Inherited from Interface
    Interface* Iself = (Interface*)self;
    Interface_destroy(Iself);
}
void delete_Level(Interface* Iself) {
    Level* self = (Level*)Iself;
    Level_destroy(self);
    al_free(self);
}

static void Level_draw(Interface* Iself, ALLEGRO_BITMAP* backbuffer) {
    Level* self = (Level*)Iself;
    if (self == nullptr) {raise_warn("Level_draw: self is nullptr"); return;}
    // Inherited from Interface
    Interface_draw(Iself, backbuffer);
    // TODO: Draw level
}
static void Level_event_recorder(Interface* Iself, ALLEGRO_EVENT event) {
    Level* self = (Level*)Iself;
    if (self == nullptr) {raise_warn("try to record event on NULL interface");return;}
    if (Iself->info.state != INTERFACE_RUNNING && Iself->info.state != INTERFACE_INITIALING) return;
    if (event.type != ALLEGRO_EVENT_KEY_DOWN) return;
    switch (event.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
        case ALLEGRO_KEY_DOWN:
        case ALLEGRO_KEY_LEFT:
        case ALLEGRO_KEY_RIGHT:
        case ALLEGRO_KEY_G:
        case ALLEGRO_KEY_R:
        case ALLEGRO_KEY_ESCAPE:
            Iself->event = event;
            break;
        default:
            break;
    }
}
static void Level_event_dealer(Interface* Iself) {
    Level* self = (Level*)Iself;
    if (Iself->event.type == NO_EVENT) return;
    switch (Iself->event.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
        case ALLEGRO_KEY_DOWN:
        case ALLEGRO_KEY_LEFT:
        case ALLEGRO_KEY_RIGHT:
        case ALLEGRO_KEY_R:
            // TODO: Deal with game event
            show_msg("TODO: Deal with game event");
            break;
        case ALLEGRO_KEY_G:
            Interface_set_stop(Iself, INTERFACE_GUIDE_MENU);
            break;
        case ALLEGRO_KEY_ESCAPE:
            Interface_set_kill(Iself, INTERFACE_NONE);
            break;
        default:
            raise_warn("try to deal with unknown event");
            break;
    }
    Iself->event.type = NO_EVENT;
}
static void Level_loader(Level* self, LEVEL_ID level_id) {
    // TODO: Load level
    show_msg("TODO: Load level");
}