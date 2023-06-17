
#include "Interface/LevelView.h"
#include "Entity/Edge.h"
#include "Entity/Apple.h"
#include "Entity/Stone.h"
#include "Entity/Box.h"
#include "Entity/Ground.h"
#include "Entity/Snake.h"

LevelView* new_Level(LEVEL_ID level_id) {
    LevelView* level = (LevelView*)al_calloc(1,sizeof(LevelView));
    Level_init(level, level_id);
    return level;
}
void Level_init(LevelView* self, LEVEL_ID level_id) {
    if (self == nullptr) {
        raise_warn("Level_init: self is nullptr"); return;}
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
    self->update_engine_period = UPDATE_ENGINE_PERIOD;
    // state
    self->PState = PLAYING;
    self->update_engine_count = 0;
    // Entity
    EntityList_init(&self->entity_list);
    EntityList_init(&self->snakes);
    // Display
    // Engine
    Level_loader(self, level_id);
}
void Level_destroy(LevelView* self) {
    if (self == nullptr) {
        raise_warn("Level_destroy: self is nullptr"); return;}
    show_msg("Level_destroy");
    // Engine
    MapEngine_destroy(&self->engine);
    // Entity
    EntityList_destroy(&self->entity_list);
    EntityList_destroy(&self->snakes);
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
    if (self == nullptr) {
        raise_warn("Level_draw: self is nullptr"); return;}
    // Inherited from Interface
    Interface_draw(Iself, backbuffer);
    // Draw level
    MapEngine_draw(&self->engine, &self->shift_window, backbuffer);
}
static void Level_event_recorder(Interface* Iself, ALLEGRO_EVENT event) {
    LevelView* self = (LevelView*)Iself;
    if (self == nullptr) {
        raise_warn("try to record event on NULL interface");return;}
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
    SW_setCenter(&self->shift_window, Level_get_view_center(self));
    if      (self->PState == WIN)  Level_deal_win(self);
    else if (self->PState == LOSE) Level_deal_lose(self);
    else if (!Level_update_counter(self)) return;
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
static bool Level_update_counter(LevelView* self) {
    if (self->update_engine_count++ < self->update_engine_period) return false;
    self->update_engine_count = 0;
    return true;
}
static Pos Level_get_view_center(LevelView* self) {
    // use snake head as center
    Snake* snake = (Snake*) self->snakes.front;
    Pos head = snake->head->object.pos;
    return add_const(head, 0.5);
}
static void Level_loader(LevelView* self, LEVEL_ID level_id) {
    // TODO: Load level, use temporary solution
    show_msg("TODO: Load level");

    // map size
    Pos map_size = make_Pos(10,10);

    // set temp entity array
    EntityArray entity_array; EntityArray_init(&entity_array);

    // set ground
    ObjectVector grounds; ObjectVector_init(&grounds);
    GroundObject ground;
    for (int i=0; i < map_size.y; i++) {
        GroundObject_init(&ground, make_Pos(map_size.x-1,i));
        ObjV_push_back(&grounds, (Object*)&ground); Object_destroy((Object*)&ground);
    }
    EntityArray_push_back(&entity_array, (Entity*)new_Ground(&grounds)); ObjectVector_destroy(&grounds);

    // set box
    ObjectVector boxs; ObjectVector_init(&boxs);
    BoxObject box1; BoxObject_init(&box1, make_Pos(4,3));
    BoxObject box2; BoxObject_init(&box2, make_Pos(4,2));
    BoxObject box3; BoxObject_init(&box3, make_Pos(5,3));
    ObjV_push_back(&boxs, (Object*)&box1); Object_destroy((Object*)&box1);
    ObjV_push_back(&boxs, (Object*)&box2); Object_destroy((Object*)&box2);
    ObjV_push_back(&boxs, (Object*)&box3); Object_destroy((Object*)&box3);
    EntityArray_push_back(&entity_array, (Entity*)new_Box(&boxs)); ObjectVector_destroy(&boxs);

    // set apple
    ObjectVector apples; ObjectVector_init(&apples);
    AppleObject apple; AppleObject_init(&apple, make_Pos(8,3));
    ObjV_push_back(&apples, (Object*)&apple); Object_destroy((Object*)&apple);
    EntityArray_push_back(&entity_array, (Entity*)new_Apple(&apples)); ObjectVector_destroy(&apples);

    // set snake
    ObjectVector bodies; ObjectVector_init(&bodies);
    BodyObject head; BodyObject_init(&head, make_Pos(0,0), HEAD, DIRECTION_UP, DIRECTION_UP);
    ObjV_push_back(&bodies, (Object*)&head); Object_destroy((Object*)&head);
    BodyObject body;
    for (int i=1; i < 5; i++) {
        BodyObject_init(&body, make_Pos(i,0), BODY, DIRECTION_UP, DIRECTION_UP);
        ObjV_push_back(&bodies, (Object*)&body); Object_destroy((Object*)&body);
    }
    BodyObject tail; BodyObject_init(&tail, make_Pos(5,0), TAIL, DIRECTION_UP, DIRECTION_UP);
    ObjV_push_back(&bodies, (Object*)&tail); Object_destroy((Object*)&tail);
    EntityList_push_back(&self->snakes, (Entity*)new_Snake(&bodies)); ObjectVector_destroy(&bodies);

    // sort temp entity array
    EntityArray_sort(&entity_array);

    // set entity list
    EntityList_from_array(&self->entity_list, &entity_array); EntityArray_destroy(&entity_array);

    // set shift window
    ShiftWindow_init(&self->shift_window, map_size, make_Pos(0,0), map_size);
    Pos window_size = make_Pos(9,9);
    //SW_window_resize(&self->shift_window, window_size);
    SW_setCenter(&self->shift_window, Level_get_view_center(self));

    // set map engine
    MapEngine_init(&self->engine, map_size, &self->entity_list, self->snakes.front);
}