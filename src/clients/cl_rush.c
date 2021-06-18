#include "clients_commons.h"
#include "strat_rush.h"
#include <stdio.h>


static char* name = "Rush";
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

  return rush_move(&game, &self);
}


void finalize()
{
  client_free(&game, &self);
}
