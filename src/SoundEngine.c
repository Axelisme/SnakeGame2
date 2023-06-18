
#include "SoundEngine.h"

static bool Mute = INIT_MUTE;
static int sample_instance_num;
static ALLEGRO_SAMPLE_INSTANCE* sample_instances[SAMPLE_INSTANCE_MAX_NUM];

void SE_init() {
    // Initial sound
    show_msg("Initial sound");
    if(!al_reserve_samples(SAMPLE_MAX_NUM))
        raise_warn("can't not initial sound");
}
void SE_destroy() {SE_clear_sound();}
void SE_update() {
    // mute or unmute the music
    al_set_mixer_playing(al_get_default_mixer(), !Mute);
    // clear no playing sound
    SE_clear_not_playing_sound();
}
void SE_add_sound(ALLEGRO_SAMPLE* sample, ALLEGRO_PLAYMODE playmode) {
    if (sample==nullptr) {
        raise_warn("can't add nullptr sound sample");return;}
    if (sample_instance_num==SAMPLE_INSTANCE_MAX_NUM) {
        raise_warn("sample instance stack overflow");return;}
    ALLEGRO_SAMPLE_INSTANCE* sample_instance = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(sample_instance, playmode);
    al_attach_sample_instance_to_mixer(sample_instance, al_get_default_mixer());
    al_play_sample_instance(sample_instance);
    sample_instances[sample_instance_num++] = sample_instance;
}
static void SE_clear_not_playing_sound() {
    for(int i=0;i<sample_instance_num;i++) {
        ALLEGRO_SAMPLE_INSTANCE* sample_instance = sample_instances[i];
        if (al_get_sample_instance_playing(sample_instance)) continue;
        al_destroy_sample_instance(sample_instance);
        int last_idx = --sample_instance_num;
        swap(sample_instances[i], sample_instances[last_idx]);
        i--;
        sample_instances[last_idx] = nullptr;
    }
}
static void SE_clear_sound() {
    for(int i=0;i<sample_instance_num;i++)
        al_destroy_sample_instance(sample_instances[i]);
    sample_instance_num = 0;
}
void SE_set_mute(bool mute)  {Mute = mute;}
bool SE_get_mute()           {return Mute;}
void SE_toggle_mute()        {Mute = !Mute;}