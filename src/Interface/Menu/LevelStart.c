
#include "Interface/Menu/LevelStart.h"
#include "GameWindow.h"

const char LEVEL_1_START_IMAGE_PATH[] = "data/image/level1_start.png";
const char LEVEL_2_START_IMAGE_PATH[] = "data/image/level2_start.png";
const char LEVEL_3_START_IMAGE_PATH[] = "data/image/level3_start.png";
const char LEVEL_4_START_IMAGE_PATH[] = "data/image/level4_start.png";


LevelStart* new_LevelStart(LEVEL_ID level_id) {
    LevelStart* self = (LevelStart*)al_calloc(1,sizeof(LevelStart));
    LevelStart_init(self, level_id);
    return self;
}
void LevelStart_init(LevelStart* self, LEVEL_ID level_id) {
    if (!self) {raise_warn("Try to init NULL level start interface");return;}
    const char* IMAGE_PATH = _LevelStart_get_image_path(level_id);
    SingleMenu_init(self, IMAGE_PATH);
    // inherited from Interface
    Interface* Iself = (Interface*)self;
    Iself->info.type = INTERFACE_LEVEL_START;
    Iself->background_light_max = 150;
    Iself->background_light_up_step = 2;
    Iself->background_light_down_step = 10;
    Iself->event_dealer = _LevelStart_deal_event;
    Iself->deleter = delete_LevelStart;
    // set info
    LevelStartInfo* info = (LevelStartInfo*)al_calloc(1,sizeof(LevelStartInfo));
    info->level = level_id;
    info->remain_time = DISPLAY_FPS*2;
    self->info = (void*)info;
}
void LevelStart_destroy(LevelStart* self) {
    if (!self) {raise_warn("Try to destroy NULL level start interface");return;}
    SingleMenu_destroy(self);
    al_free(self->info);
}
void delete_LevelStart(Interface* Iself) {
    LevelStart* self = (LevelStart*)Iself;
    LevelStart_destroy(self);
    al_free(self);
}

static void _LevelStart_deal_event(Interface* Iself) {
    LevelStart* self = (LevelStart*)Iself;
    LevelStartInfo* info = (LevelStartInfo*)self->info;
    if (Iself->event.type == ALLEGRO_EVENT_KEY_DOWN || info->remain_time-- <= 0)
        _LevelStart_enter_level(self, info->level);
}
static const char* _LevelStart_get_image_path(LEVEL_ID level_id) {
    switch (level_id) {
        case LEVEL_1:
            return LEVEL_1_START_IMAGE_PATH;
        case LEVEL_2:
            return LEVEL_2_START_IMAGE_PATH;
        case LEVEL_3:
            return LEVEL_3_START_IMAGE_PATH;
        case LEVEL_4:
            return LEVEL_4_START_IMAGE_PATH;
        default:
            raise_err("Unknown level id");
            return nullptr;
    }
}
static void _LevelStart_enter_level(LevelStart* self, LEVEL_ID level_id) {
    Interface* Iself = (Interface*)self;
    Iself->info.state = INTERFACE_EXITING;
    Iself->info.child.interface_type = INTERFACE_LEVEL;
    Iself->info.child.level = level_id;
    Iself->should_kill = true;
}
