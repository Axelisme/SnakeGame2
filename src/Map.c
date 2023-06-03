#include "Map.h"
#include <stdlib.h>

// Map
Map* new_Map() {
    Map* map = (Map*)malloc(sizeof(Map));
    Map_init(map, 0, 0);
    return map;
}
void Map_init(Map* map, const size_t y_size, const size_t x_size) {
    map->map = NULL;
    Map_resize(map, y_size, x_size);
}
void Map_resize(Map* map, const size_t y_size, const size_t x_size) {
    map->y_size = y_size;
    map->x_size = x_size;
    Map_destory(map);
    map->map = (OBJ_TYPE**)malloc(sizeof(OBJ_TYPE*) * y_size);
    for (size_t i = 0; i < y_size; i++) {
        map->map[i] = (OBJ_TYPE*)malloc(sizeof(OBJ_TYPE) * x_size);
    }
}
inline OBJ_TYPE* is(Pos pos, Map* _map) {
    return &(_map->map[(int)pos.y][(int)pos.x]);
}
void Map_destory(Map* map) {
    if (map->map == NULL) return;
    for (size_t i = 0; i < map->y_size; i++) {
        free(map->map[i]);
        map->map[i] = NULL;
    }
    free(map->map);
    map->map = NULL;
}
void delete_Map(Map* map) {
    Map_destory(map);
    free(map);
}