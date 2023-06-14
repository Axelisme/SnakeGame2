
#include "Interface/Menu/LevelWin.h"

const char LEVEL_WIN_IMAGE_PATH[] = "data/image/menu/Level_win.png";

LevelWin* new_LevelWin() {
    LevelWin* self = new_SingleMenu(LEVEL_WIN_IMAGE_PATH);
    LevelWin_init(self);
    return self;
}
void LevelWin_init(LevelWin* self) {
    if (self == nullptr) {raise_err("try to init NULL interface");return;}
    show_msg("LevelWin_init");
    // inherited from Interface
    Interface* Iself = (Interface*)self;
    Iself->info.type = INTERFACE_LEVEL_WIN;
    Iself->event_dealer = _LevelWin_deal_event;
}

static void _LevelWin_deal_event(Interface* Iself) {
    if (Iself->event.type != ALLEGRO_EVENT_KEY_DOWN) return;
    Interface_set_kill(Iself, INTERFACE_NONE);
    Iself->event.type = NO_EVENT;
}