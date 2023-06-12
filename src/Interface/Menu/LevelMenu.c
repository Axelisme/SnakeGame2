
#include "Interface/Menu/LevelMenu.h"

#define LEVEL_MENU_IMAGE_NUM 5
const char LEVEL_MENU_LEVEL_1_IMAGE_PATH[] = "data/image/menu/level_in1.png";
const char LEVEL_MENU_LEVEL_2_IMAGE_PATH[] = "data/image/menu/level_in2.png";
const char LEVEL_MENU_LEVEL_3_IMAGE_PATH[] = "data/image/menu/level_in3.png";
const char LEVEL_MENU_LEVEL_4_IMAGE_PATH[] = "data/image/menu/level_in4.png";
const char LEVEL_MENU_EXIT_IMAGE_PATH[]    = "data/image/menu/level_exit.png";

LevelMenu* new_LevelMenu() {
    const char* IMAGE_PATH[] = {
        LEVEL_MENU_LEVEL_1_IMAGE_PATH,
        LEVEL_MENU_LEVEL_2_IMAGE_PATH,
        LEVEL_MENU_LEVEL_3_IMAGE_PATH,
        LEVEL_MENU_LEVEL_4_IMAGE_PATH,
        LEVEL_MENU_EXIT_IMAGE_PATH
    };
    LevelMenu* self = (LevelMenu*)new_MultiMenu(IMAGE_PATH, LEVEL_MENU_IMAGE_NUM);
    LevelMenu_init(self);
    return self;
}
void LevelMenu_init(LevelMenu* self) {
    if (self == nullptr) {raise_warn("try to initial null LevelMenu");return;}
    show_msg("LevelMenu init");
    // Inherited from Interface
    Interface* Iself = (Interface*)self;
    Iself->info.type = INTERFACE_LEVEL_MENU;
    // Info
    self->menu_state = LEVEL_MENU_LEVEL_1;
    // method
    self->enter_state = _LevelMenu_enter_state;
}

static void _LevelMenu_enter_state(LevelMenu* self) {
    Interface* Iself = (Interface*)self;
    switch (self->menu_state) {
        case LEVEL_MENU_LEVEL_1:
        case LEVEL_MENU_LEVEL_2:
        case LEVEL_MENU_LEVEL_3:
        case LEVEL_MENU_LEVEL_4:
            Iself->should_kill = false;
            Iself->info.state = INTERFACE_EXITING;
            Iself->info.child.interface_type = INTERFACE_LEVEL_START;
            Iself->info.child.level = self->menu_state + 1;
            break;
        case LEVEL_MENU_EXIT:
            _Interface_escape(Iself);
            break;
        default:
            raise_warn("LevelMenu enter state: unexpected state");
            break;
    }
}
