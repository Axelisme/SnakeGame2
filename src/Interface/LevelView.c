
#include "Interface/LevelView.h"

LevelView* new_Level(LEVEL_ID level_id) {
    LevelView* level = (LevelView*)al_calloc(1,sizeof(LevelView));
    Level_init(level, level_id);
    return level;
}
void Level_init(LevelView* self, LEVEL_ID level_id) {
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
    // state
    self->PState = PLAYING;
    // Display
    // Engine
    ObjectClass_init();
    Level_loader(self, level_id);
}
void Level_destroy(LevelView* self) {
    if (self == nullptr) {raise_warn("Level_destroy: self is nullptr"); return;}
    show_msg("Level_destroy");
    // ObjectClass
    ObjectClass_destroy();
    // Engine
    MapEngine_destroy(&self->engine);
    // Display
    ShiftWindow_destroy(&self->shift_window);
    // Inherited from Interface
    Interface* Iself = (Interface*)self;
    Interface_destroy(Iself);
}
void delete_Level(Interface* Iself) {
    LevelView* self = (LevelView*)Iself;
    Level_destroy(self);
    al_free(self);
}

static void Level_draw(Interface* Iself, ALLEGRO_BITMAP* backbuffer) {
    LevelView* self = (LevelView*)Iself;
    if (self == nullptr) {raise_warn("Level_draw: self is nullptr"); return;}
    // Inherited from Interface
    Interface_draw(Iself, backbuffer);
    // Draw level
    MapEngine_draw(&self->engine, &self->shift_window, backbuffer);
}
static void Level_event_recorder(Interface* Iself, ALLEGRO_EVENT event) {
    LevelView* self = (LevelView*)Iself;
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
        case ALLEGRO_KEY_S:
        case ALLEGRO_KEY_ESCAPE:
            Iself->event = event;
            break;
        default:
            break;
    }
}
static void Level_event_dealer(Interface* Iself) {
    LevelView* self = (LevelView*)Iself;
    if      (self->PState == WIN)  Level_deal_win(self);
    else if (self->PState == LOSE) Level_deal_lose(self);
    else if (Iself->event.type == NO_EVENT)
        self->PState = MapEngine_process(&self->engine, OP_NONE);
    else switch (Iself->event.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
                self->PState = MapEngine_process(&self->engine, OP_MOVE_UP);
                break;
            case ALLEGRO_KEY_DOWN:
                self->PState = MapEngine_process(&self->engine, OP_MOVE_DOWN);
                break;
            case ALLEGRO_KEY_LEFT:
                self->PState = MapEngine_process(&self->engine, OP_MOVE_LEFT);
                break;
            case ALLEGRO_KEY_RIGHT:
                self->PState = MapEngine_process(&self->engine, OP_MOVE_RIGHT);
                break;
            case ALLEGRO_KEY_R:
                Level_deal_lose(self);
                break;
            case ALLEGRO_KEY_S:
                Level_deal_win(self);
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
static void Level_deal_win(LevelView* self) {
    Interface* Iself = (Interface*)self;
    if (self->level_id >= LEVEL_NUM)
        Interface_set_kill(Iself, INTERFACE_LEVEL_WIN);
    else
        Interface_set_kill(Iself, INTERFACE_LEVEL);
    Iself->info.child.level = self->level_id + 1;
}
static void Level_deal_lose(LevelView* self) {
    Interface* Iself = (Interface*)self;
    Interface_set_kill(Iself, INTERFACE_LEVEL);
    Iself->info.child.level = self->level_id;
}
static void Level_loader(LevelView* self, LEVEL_ID level_id) {
    // TODO: Load level
    show_msg("TODO: Load level");
}