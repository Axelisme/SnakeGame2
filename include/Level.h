#ifndef LEVEL_H
#define LEVEL_H

#include "global.h"
#include "Snake.h"
#include "Stone.h"
#include "Button.h"
#include "Interface.h"
#include "ObjectQueue.h"
#include "Map.h"

typedef struct Level {
    // inherit from Interface
    Interface infer;
    // properties
    int level_idx;
    int mapw;
    int maph;
    Map map;
    Map ground_map;
    Map ob_map;
    Map snake_map;
    Snake snake;
    ObjectQueue object;
    DIRCTION Gravity;
    LEVEL_STAT level_stat;

    ALLEGRO_BITMAP *  Fix_Background_image;

    ALLEGRO_BITMAP *  Ground_image;
    ALLEGRO_BITMAP *  Stone_image;
    ALLEGRO_BITMAP *  Apple_image;
    ALLEGRO_BITMAP *  Snake_head_image;
    ALLEGRO_BITMAP *  Snake_body_straight_image;
    ALLEGRO_BITMAP *  Snake_body_turn_image;
    ALLEGRO_BITMAP *  Snake_tail_image;
    ALLEGRO_BITMAP *  End_point_image;
    ALLEGRO_BITMAP *  Buttom_image;
    ALLEGRO_BITMAP *  Spike_image;
    ALLEGRO_BITMAP *  Spike_down_image;
} Level;

Level* new_Level(int level_idx);
void Level_init(Level *level, int level_idx);
void Level_destory(Interface *level);
void delete_Level(Interface *level);
void Level_draw(Interface *level);
bool Level_update(Interface *level);
bool Level_load_level(Level *level, int level_idx);
void Level_level_reset(Level *level, int level_idx);
void print_map(Level *level);
void print_map_id(Level *level);
GAME_STATE Level_key_triger(Level *self, int key);
DIRCTION KEY_TO_DIRC(int key);
bool CanMove(Level *level, Pos pos, OBJ_TYPE type, DIRCTION dir);
void draw_map(Level *level);

#endif // LEVEL_H
/*
#include "global.h"
#include "Snake.h"
#include "Stone.h"
#include "Button.h"
#include "Interface.h"
#include <list>

using namespace std;

class Level: public Interface {
public:
    void draw() override;

    // update all object
    bool update() override;

    // process trigered by key
    GAME_STATE key_triger(int key);

    // load level
    bool load_level(int);

    // reset
    void level_reset(int);

    // print level map
    void print_map();

    //level id
    int getID() {return level_idx;}

    // constructor and deletor
    Level(int);
    void destroy_level();
    ~Level();
    LEVEL_STAT level_stat = KEEP;
    DIRCTION Gravity = DOWN;

private:
    static inline DIRCTION KEY_TO_DIRC(int);

    inline OBJ_TYPE& is(Pos pos,Map&_map) {return _map.at(pos.first).at(pos.second);}
    bool CanMove(Pos,OBJ_TYPE,DIRCTION);
    void draw_map();

    int level_idx;
    int mapw;
    int maph;
    Map map;
    Map ground_map;
    Map ob_map;
    Map snake_map;
    Snake *snake;
    std::list<Object*> object;

    ALLEGRO_BITMAP *  Fix_Background_image = nullptr;

    ALLEGRO_BITMAP *  Ground_image = nullptr;
    ALLEGRO_BITMAP *  Stone_image = nullptr;
    ALLEGRO_BITMAP *  Apple_image = nullptr;
    ALLEGRO_BITMAP *  Snake_head_image = nullptr;
    ALLEGRO_BITMAP *  Snake_body_straight_image = nullptr;
    ALLEGRO_BITMAP *  Snake_body_turn_image = nullptr;
    ALLEGRO_BITMAP *  Snake_tail_image = nullptr;
    ALLEGRO_BITMAP *  End_point_image = nullptr;
    ALLEGRO_BITMAP *  Buttom_image = nullptr;
    ALLEGRO_BITMAP *  Spike_image = nullptr;
    ALLEGRO_BITMAP *  Spike_down_image = nullptr;
};
*/