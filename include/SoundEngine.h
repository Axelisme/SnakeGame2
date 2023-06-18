#ifndef SOUND_ENGINE_H
#define SOUND_ENGINE_H

#include "global.h"

#define SAMPLE_MAX_NUM 10
#define SAMPLE_INSTANCE_MAX_NUM 20
#define INIT_MUTE false

void SE_init();
void SE_destroy();
void SE_update();
void SE_add_sound(ALLEGRO_SAMPLE* sample, ALLEGRO_PLAYMODE playmode);
static void SE_clear_not_playing_sound();
static void SE_clear_sound();
void SE_set_volume(float volume);
float SE_get_volume();
void SE_set_mute(bool mute);
bool SE_get_mute();
void SE_toggle_mute();

#endif // SOUND_ENGINE_H