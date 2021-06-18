#include "player.h"
#include "struct_player.h"
#include "play.h"
#include <math.h>
#include <stdlib.h>

struct player player;

/* Access to player informations
 * RETURNS:
 * - the player name as an [a-zA-Z0-9 -_]* string
 */
char const* get_player_name(){
    return player.name;
}


/* Player initialization
 * PARAM:
 * - id        : the color assigned to the player
 * - graph     : the graph where the game is played
 * - num_walls : the number of walls assigned to the player
 * PRECOND:
 * - `id` is either BLACK or WHITE
 * - `graph` is a heap-allocated copy of the graph where the game is
 *   played, that must be freed in the end
 * - `num_walls` is the number of edges of `graph` divided by 15,
     rounded up
 * - initialize has never been called before
 */
void initialize(enum color_t id, struct graph_t* graph, size_t num_walls){
    player.name = get_player_name();
    player.color = id;
    player.nb_walls = num_walls;
    player.graph = graph;
    int tab[graph->num_vertices];
    int ind = 0;
    for (int i=0; i<graph->num_vertices; i++){
        if ( (int) gsl_spmatrix_uint_get(graph->o,id,i) == 1){
            tab[ind] = i;
            ind++;
        }
    }
    player.pos[id] = tab[rand() % ind];
}


//Verify if player can move from i to j vertice.
int can__move(size_t i, size_t j){
    int n = player.graph->num_vertices;
    int m = (int) round(sqrt(m));
    if ( j < 0 || j > n-1 ) {
      return -1;
    }
    if ( player.pos[1-player.color] != j ){ 
        if ( player.pos[1-player.color] == (i+j)/2 ){
          return (   (gsl_spmatrix_uint_get(player.graph->t, (i+j)/2 , j) > 0)
                  && (gsl_spmatrix_uint_get(player.graph->t, i, (i+j)/2) > 0) );
        }
        else{
            if (j == i-m-1 || j == i-m+1){
                return (   (gsl_spmatrix_uint_get(player.graph->t, i-m, j) > 0) 
                        && (gsl_spmatrix_uint_get(player.graph->t, i-m, i-2*m) == 0)
                        && (player.pos[1-player.color] == i-m) ); 
            }
            if (j == i+m-1 || j == i+m+1){
                return (   (gsl_spmatrix_uint_get(player.graph->t, i+m, j) > 0) 
                        && (gsl_spmatrix_uint_get(player.graph->t, i+m, i+2*m) == 0)
                        && (player.pos[1-player.color] == i+m) ); 
            }
        }
        return (gsl_spmatrix_uint_get(player.graph->t, i, j) > 0 );
    }
    return -1;
}

int not_block() {
  return 1;
}



//Verify if the player can place a wall in an edge
int can__put_wall(struct edge_t e[2]){
  if((gsl_spmatrix_uint_get(player.graph->t, e[0].fr, e[0].to) > 0) 
     && (gsl_spmatrix_uint_get(player.graph->t, e[1].fr, e[1].to) > 0)) {
    if((gsl_spmatrix_uint_get(player.graph->t, e[0].fr, e[1].fr) > 0) 
       && (gsl_spmatrix_uint_get(player.graph->t, e[0].to, e[1].to) > 0)){
      if(not_block()) {
      return 1;
      }
    }
  }
  return -1;
}


//Creates a struct move_t for the move to the vertex m
struct move_t new_move(size_t pos){
    struct move_t mv = empty_move();
    mv.t = MOVE;
    mv.c = player.color;
    mv.e[0].fr = -1;
    mv.e[0].to = -1;
    mv.e[1].fr = -1;
    mv.e[1].to = -1;
    mv.m = pos;
    return mv;
}



//Creates a struct move_t for the wall placement on edges e[0] and e[1]
struct move_t new_wall(struct edge_t e[2]){
    struct move_t mv = empty_move();
    mv.t = WALL;
    mv.c = player.color;
    mv.e[0] = e[0];
    mv.e[1] = e[1];
    mv.m = -1;
    return mv;
}



//Fills possible movements in tab and return its number
int fill_moves(struct move_t tab[], int size){
    int pos = player.pos[player.color];
    int n = player.graph->num_vertices;
    int m = (int) round(sqrt(n)); 


    //MOVES (7 sommets au max)

    if (can__move(pos,pos+1)){
        struct move_t mv = new_move(pos+1);
        tab[size] = mv;
        size++;  
    }
    else // player.pos[1-player.color] == pos+1
    {
        if (can__move(pos,pos+2)){
            struct move_t mv = new_move(pos+2);
            tab[size] = mv;
            size++;
        }
        else // a wall exists between pos+1 and pos+2
        {
            if (can__move(pos,pos+1+m)){
                struct move_t mv = new_move(pos+1+m);
                tab[size] = mv;
                size++;
            }
            if (can__move(pos,pos+1-m)){
                struct move_t mv = new_move(pos+1-m);
                tab[size] = mv;
                size++;
            }
        }
    }


    if (can__move(pos,pos-1)){
        struct move_t mv = new_move(pos-1);
        tab[size] = mv;
        size++;
    }
    else // player.pos[1-player.color] == pos-1
    {
        if (can__move(pos,pos-2)){
            struct move_t mv = new_move(pos-2);
            tab[size] = mv;
            size++;
        }
        else // a wall exists between pos-1 and pos-2
        {
            if (can__move(pos,pos+m-1)){
                struct move_t mv = new_move(pos+m-1);
                tab[size] = mv;
                size++;
            }
            if (can__move(pos,pos-m-1)){
                struct move_t mv = new_move(pos-m-1);
                tab[size] = mv;
                size++;
            }
        }
    }


    if (can__move(pos,pos+m)){
        struct move_t mv = new_move(pos+m);
        tab[size] = mv;
        size++;
    }
    else // player.pos[1-player.color] == pos+m
    {
        if (can__move(pos,pos+2*m)){
            struct move_t mv = new_move(pos+2*m);
            tab[size] = mv;
            size++;
        }
        else // a wall exists between pos+m and pos+2*m
        {
            if (can__move(pos,pos+m+1)){
                struct move_t mv = new_move(pos+m+1);
                tab[size] = mv;
                size++;
            }
            if (can__move(pos,pos+m-1)){
                struct move_t mv = new_move(pos+m-1);
                tab[size] = mv;
                size++;
            }
        }
    }


    if (can__move(pos,pos-m)){
        struct move_t mv = new_move(pos-m);
        tab[size] = mv;
        size++;  
    }
    else // player.pos[1-player.color] == pos-m
    {
        if (can__move(pos,pos-2*m)){
            struct move_t mv = new_move(pos-2*m);
            tab[size] = mv;
            size++;
        }
        else // a wall exists between pos-m and pos-2*m
        {
            if (can__move(pos,pos-m+1)){
                struct move_t mv = new_move(pos-m+1);
                tab[size] = mv;
                size++;
            }
            if (can__move(pos,pos-m-1)){
                struct move_t mv = new_move(pos-m-1);
                tab[size] = mv;
                size++;
            }
        }
    } 
    
    return size;
}
    





//Fills possible movements in tab and return its number
int fill_walls(struct move_t tab[], int size) {
  int n = player.graph->num_vertices;
  int m = (int) round(sqrt(n)); 

  for(size_t i = 0; i < m-1; i++) {
    for(size_t j = 0; j < m-1; j++) {
      struct edge_t edges[2] = { {i*m+j, (i+1)*m+j}, {i*m+j+1, (i+1)*i+j+1}};
      if(can__put_wall(edges)) {
        struct move_t mv = new_wall(edges);
        tab[size] = mv;
        size++;
      }
    }
  }

  for(size_t i = 0; i < m-1; i++) {
    for(size_t j = 0; j < m-1; j++) {
      struct edge_t edges[2] = { {i*m+j, i*m+j+1}, {(i+1)*m+j, (i+1)*i+j+1}};
      if(can__put_wall(edges)) {
        struct move_t mv = new_wall(edges);
        tab[size] = mv;
        size++;
      }
    }
  }
  return size;
}
  






//Fills all the possible choices in tab and return size of tab
int fill_choices(struct move_t tab[]){
    int nb_moves = fill_moves(tab,0);
    int nb_walls_placements= fill_walls(tab,nb_moves);
    return nb_moves+nb_walls_placements;
}


//Gets randomly a choice from all the possible choices
struct move_t get_choice(struct move_t tab[], int size){
    return tab[ rand() % size];
}


/* Computes next move
 * PARAM:
 * - previous_move: the move from the previous player
 * RETURNS:
 * - the next move for the player.
 */
struct move_t play(struct move_t previous_move){
    if (previous_move.t = WALL){
        gsl_spmatrix_uint_set(player.graph->t,previous_move.e[0].fr,previous_move.e[0].to,0);
        gsl_spmatrix_uint_set(player.graph->t,previous_move.e[0].to,previous_move.e[0].fr,0);
        gsl_spmatrix_uint_set(player.graph->t,previous_move.e[1].fr,previous_move.e[1].to,0);
        gsl_spmatrix_uint_set(player.graph->t,previous_move.e[1].to,previous_move.e[1].fr,0);
    }
    else{ //if previous_move was a move
        player.pos[1-player.color] = previous_move.m;
    }
    struct move_t tab_choices[2*(player.graph->num_vertices)+5]; //List 
    int nb_choices = fill_choices(tab_choices);
    struct move_t move = get_choice(tab_choices, nb_choices);
    if (move.t = WALL){
        gsl_spmatrix_uint_set(player.graph->t,move.e[0].fr,move.e[0].to,0);
        gsl_spmatrix_uint_set(player.graph->t,move.e[0].to,move.e[0].fr,0);
        gsl_spmatrix_uint_set(player.graph->t,move.e[1].fr,move.e[1].to,0);
        gsl_spmatrix_uint_set(player.graph->t,move.e[1].to,move.e[1].fr,0);
        player.nb_walls--;
    }
    else{
        player.pos[player.color] = move.m;
    }
    return move;
}


/* Announces the end of the game to the player, and cleans up the
   memory he may have been using.
 * POSTCOND:
 * - every allocation done during the calls to initialize and play
 *   functions must have been freed
 */
void finalize();
