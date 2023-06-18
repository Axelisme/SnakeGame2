
#include <stdio.h>
#include "Utility/LevelRead.h"
#include "Interface/LevelView.h"
#include "Object/SimpleObject.h"
#include "Entity/Edge.h"
#include "Entity/Apple.h"
#include "Entity/Button.h"
#include "Entity/Stone.h"
#include "Entity/Box.h"
#include "Entity/End.h"
#include "Entity/Ground.h"
#include "Entity/Spike.h"
#include "Entity/Snake.h"

static const char LEVEL_PATH[] = "data/level";
static const char DIED_IMAGE_PATH[] = "data/image/menu/died.png";
static const char BACKGROUND_IMAGE_PATH[] = "data/image/background.png";

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
    Iself->update = Level_update;
    Iself->event_record = Level_event_recorder;
    Iself->event_dealer = Level_event_dealer;
    Iself->deleter = delete_Level;
    // Info
    self->level_id = level_id;
    self->update_engine_period = UPDATE_ENGINE_PERIOD;
    self->background_move_speed = BACKGROUND_MOVE_SPEED;
    // state
    self->PState = PLAYING;
    self->update_engine_count = 0;
    self->died_image_opacity = 0;
    self->died_image_opacity_step = DIED_STEP;
    // Entity
    EntityList_init(&self->entity_list);
    EntityList_init(&self->snakes);
    // Display
    self->died_image = al_load_bitmap(DIED_IMAGE_PATH);
    self->background = al_load_bitmap(BACKGROUND_IMAGE_PATH);
    if (!self->died_image) raise_warn("Level_init: failed to load died image");
    if (!self->background) raise_warn("Level_init: failed to load background image");
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
    if (self->died_image) al_destroy_bitmap(self->died_image);
    if (self->background) al_destroy_bitmap(self->background);
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
    // Draw background
    SW_draw_background(&self->shift_window, backbuffer, self->background, self->background_move_speed);
    // Draw level
    MapEngine_draw(&self->engine, &self->shift_window, backbuffer);
    // Draw Died
    if (self->PState == LOSE)
        draw_image(self->died_image, backbuffer, DIRECTION_UP, self->died_image_opacity);
}
static INTERFACE_INFO Level_update(Interface* Iself) {
    LevelView* self = (LevelView*)Iself;
    if (Iself == nullptr) {raise_warn("try to update NULL interface");return _fall_back_info();}
    switch (Iself->info.state) {
        case INTERFACE_INITIALING:
            if (Interface_light_up(Iself))
                Iself->info.state = INTERFACE_RUNNING;
            break;
        case INTERFACE_RUNNING:
            if (Level_update_counter(self))
                Iself->event_dealer(Iself);
            break;
        case INTERFACE_EXITING:
            if (self->PState == LOSE && !Level_update_died_opacity(self))
                break;
            if (Interface_light_down(Iself))
                Iself->info.state = (Iself->should_kill)? INTERFACE_DIED: INTERFACE_STOP;
            break;
        case INTERFACE_STOP:
            Iself->info.state = INTERFACE_INITIALING;
            break;
        case INTERFACE_DIED:
            break;
        default:
            raise_err("unknown interface state");
            break;
    }
    return Iself->info;
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
        case ALLEGRO_KEY_M:
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
            case ALLEGRO_KEY_M:
                Interface_set_stop(Iself, INTERFACE_MUSIC_MENU);
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
static bool Level_update_died_opacity(LevelView* self) {
    if (self->died_image_opacity >= 255) return true;
    self->died_image_opacity += self->died_image_opacity_step;
    if (self->died_image_opacity > 255) self->died_image_opacity = 255;
    return false;
}
static Pos Level_get_view_center(LevelView* self) {
    // use snake head as center
    Snake* snake = (Snake*) self->snakes.front;
    Pos head = snake->head->object.pos;
    return add_const(head, 0.5);
}
static void Level_load_default_level(LevelView* self) {
    // map size
    Pos map_size = make_Pos(10,20);

    // set ground
    ObjectVector grounds; ObjectVector_init(&grounds);
    for (int i=0; i < map_size.x; i++) {
        GroundObject ground; GroundObject_init(&ground, make_Pos(map_size.y-1,i));
        ObjV_push_back(&grounds, (Object*)&ground); Object_destroy((Object*)&ground);
    }
    EntityList_insert(&self->entity_list, (Entity*)new_Ground(&grounds)); ObjectVector_destroy(&grounds);

    // set spike
    ObjectVector spikes; ObjectVector_init(&spikes);
    for (int i=6; i < 8; i++) {
        GroundObject spike; SpikeObject_init(&spike, make_Pos(map_size.y-2,i));
        ObjV_push_back(&spikes, (Object*)&spike); Object_destroy((Object*)&spike);
    }
    EntityList_insert(&self->entity_list, (Entity*)new_Spike(&spikes)); ObjectVector_destroy(&spikes);

    // set box
    ObjectVector boxs; ObjectVector_init(&boxs);
    BoxObject box1; BoxObject_init(&box1, make_Pos(4,3));
    BoxObject box2; BoxObject_init(&box2, make_Pos(4,2));
    BoxObject box3; BoxObject_init(&box3, make_Pos(5,3));
    ObjV_push_back(&boxs, (Object*)&box1); Object_destroy((Object*)&box1);
    ObjV_push_back(&boxs, (Object*)&box2); Object_destroy((Object*)&box2);
    ObjV_push_back(&boxs, (Object*)&box3); Object_destroy((Object*)&box3);
    EntityList_insert(&self->entity_list, (Entity*)new_Box(&boxs)); ObjectVector_destroy(&boxs);

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
    EntityList_insert(&self->snakes, (Entity*)new_Snake(&bodies)); ObjectVector_destroy(&bodies);

    // set shift window
    ShiftWindow_init(&self->shift_window, map_size, make_Pos(0,0), make_Pos(10,20));
    Pos window_size = make_Pos(10, 10);
    SW_window_resize(&self->shift_window, window_size);
    SW_setCenter(&self->shift_window, Level_get_view_center(self));

    // set map engine
    MapEngine_init(&self->engine, map_size, &self->entity_list, self->snakes.front);
}
static void Level_loader(LevelView* self, LEVEL_ID level_id) {
    // get level path
    char level_path[MAX_PATH_LEN];
    sprintf(level_path, "%s/Level%d", LEVEL_PATH, level_id);
    show_msg("Load level:");
    show_msg(level_path);

    // load level
    FILE* fp = fopen(level_path, "r");
    if (!fp) {
        raise_warn("fail to open level file, use default level");
        Level_load_default_level(self); return;
    }
    char line[MAX_LINE_LEN];

    // read map info
    Pos map_size;
    readline(fp, line, MAX_LINE_LEN); sscanf(line, "%lf %lf", &map_size.y, &map_size.x);
    Pos view_UL, view_LR;
    readline(fp, line, MAX_LINE_LEN); sscanf(line, "%lf %lf", &view_UL.y, &view_LR.y);
    readline(fp, line, MAX_LINE_LEN); sscanf(line, "%lf %lf", &view_UL.x, &view_LR.x);
    ShiftWindow_init(&self->shift_window, map_size, view_UL, view_LR);
    Pos window_size;
    readline(fp, line, MAX_LINE_LEN); sscanf(line, "%lf %lf", &window_size.y, &window_size.x);
    SW_window_resize(&self->shift_window, window_size);
    Direction gravity;
    readline(fp, line, MAX_LINE_LEN); sscanf(line, "%d", &gravity);

    // read snake info
    int snake_num;
    readline(fp, line, MAX_LINE_LEN); sscanf(line, "%d", &snake_num);
    if (snake_num != 1) {
        raise_err("snake more than one isn't implemented yet");
        fclose(fp);
        Level_load_default_level(self);
        return;
    }
    ObjectVector bodies; ObjectVector_init(&bodies);
    int body_num;
    readline(fp, line, MAX_LINE_LEN); sscanf(line, "%d", &body_num);
    if (body_num < 3) {
        raise_err("snake body number less than 3");
        fclose(fp);
        Level_load_default_level(self);
        return;
    }
    Pos pos;
    Direction dir, next_dir = DIRECTION_UP;
    readline(fp, line, MAX_LINE_LEN); sscanf(line, "%lf %lf %d", &pos.y, &pos.x, &dir);
    BodyObject head; BodyObject_init(&head, pos, HEAD, dir, next_dir);
    ObjV_push_back(&bodies, (Object*)&head); Object_destroy((Object*)&head);
    next_dir = dir;
    for (int i=1; i < body_num-1; i++) {
        readline(fp, line, MAX_LINE_LEN); sscanf(line, "%lf %lf %d", &pos.y, &pos.x, &dir);
        BodyObject body; BodyObject_init(&body, pos, BODY, dir, next_dir);
        ObjV_push_back(&bodies, (Object*)&body); Object_destroy((Object*)&body);
        next_dir = dir;
    }
    readline(fp, line, MAX_LINE_LEN); sscanf(line, "%lf %lf %d", &pos.y, &pos.x, &dir);
    BodyObject tail; BodyObject_init(&tail, pos, TAIL, dir, next_dir);
    ObjV_push_back(&bodies, (Object*)&tail); Object_destroy((Object*)&tail);
    EntityList_insert(&self->snakes, (Entity*)new_Snake(&bodies)); ObjectVector_destroy(&bodies);

    // read entities
    int entity_num;
    readline(fp, line, MAX_LINE_LEN); sscanf(line, "%d", &entity_num);
    if (entity_num < 0) {
        raise_err("entity number less than 0");
        fclose(fp);
        Level_load_default_level(self);
        return;
    }
    for (int i=0; i < entity_num; i++) {
        Entity* entity = _create_Entity(fp);
        if (!entity) {
            raise_err("fail to create entity");
            fclose(fp);
            Level_load_default_level(self);
            return;
        }
        EntityList_insert(&self->entity_list, entity);
    }

    // close file
    fclose(fp);

    // set map center
    SW_setCenter(&self->shift_window, Level_get_view_center(self));

    // set map engine
    MapEngine_init(&self->engine, map_size, &self->entity_list, self->snakes.front);
    ME_setGravity(&self->engine, gravity);
}
static Entity* _create_Entity(FILE* fp) {
    char line[MAX_LINE_LEN], temp[MAX_LINE_LEN];
    EntityType type;
    int obj_num;
    readline(fp, line, MAX_LINE_LEN); sscanf(line, "%d%s %d", &type, temp, &obj_num);
    ObjectCreator object_creator;
    EntityCreator entity_creator;
    _get_Creator(type, &object_creator, &entity_creator);
    Pos pos;
    Direction dir;
    ObjectVector objs; ObjectVector_init(&objs);
    for (int i=0; i < obj_num; i++) {
        readline(fp, line, MAX_LINE_LEN); sscanf(line, "%lf %lf %d", &pos.y, &pos.x, &dir);
        Object* obj = object_creator(pos);
        obj->dir = dir;
        ObjV_push_back(&objs, obj);
        obj->deleter(obj);
    }
    Entity* entity = entity_creator(&objs); ObjectVector_destroy(&objs);
    int other_info;
    readline(fp, line, MAX_LINE_LEN); sscanf(line, "%d", &other_info);
    if (other_info)
        entity->setter(entity, fp);
    return entity;
}
static void _get_Creator(EntityType type, ObjectCreator* creator, EntityCreator* entity_creator) {
    switch (type) {
        case E_EDGE :
            *creator = (ObjectCreator)new_EdgeObject;
            *entity_creator = (EntityCreator)new_Edge;
            break;
        case E_APPLE :
            *creator = (ObjectCreator)new_AppleObject;
            *entity_creator = (EntityCreator)new_Apple;
            break;
        case E_BUTTON :
            *creator = (ObjectCreator)new_ButtonObject;
            *entity_creator = (EntityCreator)new_Button;
            break;
        case E_SPIKE :
            *creator = (ObjectCreator)new_SpikeObject;
            *entity_creator = (EntityCreator)new_Spike;
            break;
        case E_END :
            *creator = (ObjectCreator)new_EndObject;
            *entity_creator = (EntityCreator)new_End;
            break;
        case E_BOX :
            *creator = (ObjectCreator)new_BoxObject;
            *entity_creator = (EntityCreator)new_Box;
            break;
        case E_STONE :
            *creator = (ObjectCreator)new_StoneObject;
            *entity_creator = (EntityCreator)new_Stone;
            break;
        case E_GROUND :
            *creator = (ObjectCreator)new_GroundObject;
            *entity_creator = (EntityCreator)new_Ground;
            break;
        default :
            raise_err("unknown entity type");
            *creator = NULL;
            *entity_creator = NULL;
    }
}