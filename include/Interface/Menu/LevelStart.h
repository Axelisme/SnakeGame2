#ifndef LEVEL_START_H
#define LEVEL_START_H

#include "Interface/Menu/SingleMenu.h"
#include "Interface/Level.h"

typedef struct LevelStartInfo {
    LEVEL_ID level;
    int remain_time;
} LevelStartInfo;

typedef SingleMenu LevelStart;

LevelStart* new_LevelStart(LEVEL_ID level_id);
void LevelStart_init(LevelStart* self, LEVEL_ID level_id);
void LevelStart_destroy(LevelStart* self);
void delete_LevelStart(Interface* Iself);

static void _LevelStart_deal_event(Interface* Iself);
static const char* _LevelStart_get_image_path(LEVEL_ID level_id);
static void _LevelStart_enter_level(LevelStart* self, LEVEL_ID level_id);

#endif // LEVEL_START_H