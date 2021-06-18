#include "clients_commons.h"
#include "strat_rush.h"
#include "strat_greedy.h"
#include <stdio.h>


static char* name = "Greedy";
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


struct move_t play(struct move_t previous_move)
{
  update_game(&game, previous_move);
  
  if (game.pl_pos[self.id] == SIZE_MAX) // starting position
      return first_move(&game, &self);

  if (game.turn > 2 && greedy_score(&game, &self) < 1 && game.nb_walls[self.id])
    {
      struct move_t move = greedy_wall(&game, &self);
      if (move.t == NO_TYPE)
	return rush_move(&game, &self);
      else
	return move;
    }
  return rush_move(&game, &self);	
}


void finalize()
{
  client_free(&game, &self);
}
