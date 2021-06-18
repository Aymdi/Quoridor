#ifndef _PLAY_H_
#define _PLAY_H_

#include "move.h"
#include "graph.h"

struct move_t empty_move(){
    struct move_t empty_move = {};
    return empty_move;
}

//Verify if player can move from i to j vertice.
int can__move(size_t i, size_t j);

//Verify if the player can place a wall in an edge
int can__put_wall(struct edge_t e[2]);

int not_block();

//Fills possible movements in tab and return its number
int fill_moves(struct move_t tab[], int size);

//Fills possible wall placements in tab and return its number
int fill_walls(struct move_t tab[], int size);

//Fills all the possible choices in tab and return size of tab
int fill_choices(struct move_t tab[]);

//Creates a struct move_t for the move to the vertex m
struct move_t new_move(size_t pos);

//Creates a struct move_t for the wall placement on edges e[0] and e[1]
struct move_t new_wall(struct edge_t e[2]);

//Gets randomly a choice from all the possible choices
struct move_t get_choice(struct move_t tab[], int size);


#endif //_PLAY_H_