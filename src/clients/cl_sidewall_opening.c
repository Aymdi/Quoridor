#include "clients_commons.h"
#include "strat_rush.h"
#include "strat_greedy.h"
#include <stdio.h>

static char* name = "Sidewall opening";
static struct player self;
static struct game_state game;


char const* get_player_name()
{
    return name;
}


void initialize(enum color_t id, struct graph_t* graph, size_t num_walls)
{
    client_init(id, graph, num_walls, &game, &self);
}





struct move_t play_greedy()
{ 
    if (greedy_score(&game, &self) < 1 && game.nb_walls[self.id])
    {
        struct move_t move = greedy_wall(&game, &self);
        if (move.t == NO_TYPE)
	        return rush_move(&game, &self);
        else
	        return move;
    }
    return rush_move(&game, &self);	
}






struct move_t play(struct move_t previous_move)
{
    update_game(&game, previous_move);
  
    if (game.pl_pos[self.id] == SIZE_MAX) // starting position
        return first_move(&game, &self);

    int t;

    if(self.id == BLACK) // if the player starts second, the strategy is done one turn earlier
        t = game.turn;
    else    
        t = game.turn + 1;

    
    int m = game.board_width;    
    int n = game.graph->num_vertices;
    struct edge_t wall[2];
    size_t mv;


    switch(t)
    {
        case 2:
            game.pl_pos[self.id] += m;
            return (struct move_t) {game.pl_pos[self.id], {no_edge(), no_edge()}, MOVE, self.id};
        case 3:
            if(self.id == BLACK)
            {
                wall[0].fr = n-1-m/2-m;
                wall[0].to = n-1-m/2-m-1; 
                wall[1].fr = n-1-m/2-2*m;
                wall[1].to = n-1-m/2-2*m-1;
            }
            else
            {    
                wall[0].fr = m/2+m;
                wall[0].to = m/2+m-1;
                wall[1].fr = m/2+2*m;
                wall[1].to = m/2+2*m-1;
            }
            if(wall_is_licit(&game,wall) && game.nb_walls[self.id])
            {
                place_wall(game.graph,wall);
                game.nb_walls[self.id]--;
                return (struct move_t) {SIZE_MAX, {wall[0], wall[1]}, WALL, self.id};
            }
            else
                return play_greedy();
            break;
        case 4:
            if(self.id == BLACK)
            { 
                wall[0].fr = n-1-m/2-2*m;
                wall[0].to = n-1-m/2-3*m; 
                wall[1].fr = n-1-m/2-2*m+1;
                wall[1].to = n-1-m/2-3*m+1;
            }
            else    
            { 
                wall[0].fr = m/2+2*m;
                wall[0].to = m/2+3*m;
                wall[1].fr = m/2+2*m+1;
                wall[1].to = m/2+3*m+1;
            }
            if(wall_is_licit(&game,wall) && game.nb_walls[self.id])
            {
                place_wall(game.graph,wall);
                game.nb_walls[self.id]--;
                return (struct move_t) {SIZE_MAX, {wall[0], wall[1]}, WALL, self.id};
            }
            else
                return play_greedy();
        case 5:
            if(self.id == BLACK)
                mv = game.pl_pos[self.id] + m;
            else
                mv = game.pl_pos[self.id] - m;
            if(move_is_licit(&game,self.id,mv))
            { 
                game.pl_pos[self.id] = mv;
                return (struct move_t) {mv, {no_edge(), no_edge()}, MOVE, self.id};
            }
            else
                return play_greedy();
        case 6:
            if(self.id == BLACK)
            { 
                wall[0].fr = n-1-m/2-2*m+2;
                wall[0].to = n-1-m/2-3*m+2;
                wall[1].fr = n-1-m/2-2*m+3;
                wall[1].to = n-1-m/2-3*m+3;
            }
            else    
            { 
                wall[0].fr = m/2+2*m+2;
                wall[0].to = m/2+3*m+2;
                wall[1].fr = m/2+2*m+3;
                wall[1].to = m/2+3*m+3;
            }
            if(wall_is_licit(&game,wall) && game.nb_walls[self.id])
            {
                place_wall(game.graph,wall);
                game.nb_walls[self.id]--; 
                return (struct move_t) {SIZE_MAX, {wall[0], wall[1]}, WALL, self.id};
            }
            else
                return play_greedy();
        case 7:
            if(self.id == BLACK)
            {
                wall[0].fr = n-1-m/2-m+4;
                wall[0].to = n-1-m/2-m+3;
                wall[1].fr = n-1-m/2-2*m+4;
                wall[1].to = n-1-m/2-2*m+3;
            }   
            else    
            {
                wall[0].fr = m/2+m+4; 
                wall[0].to = m/2+m+3;
                wall[1].fr = m/2+2*m+4;
                wall[1].to = m/2+2*m+3;
            }
            if(wall_is_licit(&game,wall) && game.nb_walls[self.id])
            {
                place_wall(game.graph,wall);
                game.nb_walls[self.id]--;
                return (struct move_t) {SIZE_MAX, {wall[0], wall[1]}, WALL, self.id};
            }
            else
                return play_greedy();
        default:
            return play_greedy();
    }
      
}





void finalize()
{
    client_free(&game, &self);
}
