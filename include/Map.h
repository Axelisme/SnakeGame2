#ifndef MAP_H
#define MAP_H

#include "Object.h"
#include "Position.h"

// Map
typedef struct Map
{
    size_t y_size;
    size_t x_size;
    OBJ_TYPE **map;
} Map;

Map* new_Map();
void Map_init(Map* map, const size_t y_size, const size_t x_size);
void Map_resize(Map* map, const size_t y_size, const size_t x_size);
OBJ_TYPE* is(Pos pos, Map* _map);
void Map_destory(Map* map);
void delete_Map(Map* map);

#endif // MAP_H