
#include "Entity/Button.h"
#include "Utility/LevelRead.h"
#include "Engine.h"
#include "Entity/Spike.h"
#include "Entity/Snake.h"
#include "SoundEngine.h"

Entity* new_Button(ObjectVector* objs) {
    Button* self = (Button*)malloc(sizeof(Button));
    Button_init(self, objs);
    return (Entity*)self;
}
void Button_init(Button* self, ObjectVector* objs) {
    Entity* Eself = (Entity*)self;
    Entity_init(Eself, objs);
    Eself->type = E_BUTTON;
    Eself->isFixed = true;
    Eself->canOverlap = true;
    Eself->setter = Button_setter;
    Eself->trigger = Button_trigger;
    self->isPressed = false;
    self->effect = SNAKE_SHRINK;
    self->effect_info.snakeS.shrink_num = 0;
    self->effect_repeat = 0;
    self->effect_remain = self->effect_repeat;
}

static void Button_setter(Entity* Eself, FILE* fp) {
    Button* self = (Button*)Eself;
    char line[EFFECT_LINE_MAX];
    readline(fp, line, EFFECT_LINE_MAX); sscanf(line, "%d", &self->effect);
    readline(fp, line, EFFECT_LINE_MAX); sscanf(line, "%d", &self->effect_repeat);
    self->effect_remain = self->effect_repeat;
    switch (self->effect) {
        case SNAKE_SHRINK:
            readline(fp, line, EFFECT_LINE_MAX);
            sscanf(line, "%d", &self->effect_info.snakeS.shrink_num);
            break;
        case GRAVITY_CHANGE:
            readline(fp, line, EFFECT_LINE_MAX);
            sscanf(line, "%d", &self->effect_info.gravityR.gravity);
            break;
        case SPIKE_SET:
            readline(fp, line, EFFECT_LINE_MAX); sscanf(line, "%d", &self->effect_info.spikeS.spike_num);
            for (int i = 0; i < self->effect_info.spikeS.spike_num; i++) {
                readline(fp, line, EFFECT_LINE_MAX);
                sscanf(line, "%lf %lf %d",
                        &self->effect_info.spikeS.spike_pos[i].y,
                        &self->effect_info.spikeS.spike_pos[i].x,
                        &self->effect_info.spikeS.spike_dir[i]);
            }
            break;
        default:
            raise_warn("Button_setter: unknown effect type");
    }
}
static void Button_trigger(Entity* Eself, MapEngine* Engine, EntityMap* Map, EntityArray* overlaps) {
    Button* self = (Button*)Eself;
    if (self->isPressed) return;
    if (!self->effect_remain--) {
        self->isPressed = true;
        ButtonObject_down((ButtonObject*)ObjV_get(&Eself->objList, 0));
    }
    SE_add_sound(button_down_sound, ALLEGRO_PLAYMODE_ONCE);
    switch (self->effect) {
        case SNAKE_SHRINK:
            for (int i = 0; i < self->effect_info.snakeS.shrink_num; i++)
                Snake_shrink((Snake*)Engine->snake, Map, overlaps);
            break;
        case GRAVITY_CHANGE:
            Engine->gravity = self->effect_info.gravityR.gravity;
            break;
        case SPIKE_SET: {
            int spike_num = self->effect_info.spikeS.spike_num;
            ObjectVector obj; ObjectVector_init(&obj);
            for (int i = 0; i < spike_num; i++) {
                Pos spike_pos = self->effect_info.spikeS.spike_pos[i];
                Direction spike_dir = self->effect_info.spikeS.spike_dir[i];
                Entity* origin = MapGet(Map, spike_pos);
                if (!origin) {
                    SpikeObject* spike = new_SpikeObject(spike_pos);
                    spike->dir = spike_dir;
                    ObjV_push_back(&obj, (Object*)spike);
                    ((Object*)spike)->deleter((Object*)spike);
                }
                else if (origin->type == E_SPIKE)
                    origin->Alive = false;
                else {
                    raise_warn("Button_trigger: spike position is not empty");
                    continue;
                }
            }
            Entity* Espike = new_Spike(&obj);
            Entity_mark(Espike, Map, overlaps);
            EntityList_insert(Engine->entities, Espike);
            break;
        }
        default:
            raise_warn("Button_trigger: unknown effect type");
    }
}