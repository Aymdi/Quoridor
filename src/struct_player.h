#ifndef _STRUCT_PLAYER_H
#define _STRUCT_PLAYER_H

#include "graph.h"
#include "move.h"




struct player{
    char const *name;
    enum color_t color;
    size_t nb_walls;
    int pos[2];
    struct graph_t *graph;
};









#endif //_STRUCT_PLAYER_H