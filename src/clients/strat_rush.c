#include "strat_rush.h"

/* A move that shortens a player's shortest path to the other side */
struct move_t rush_move(struct game_state* game, struct player* p)
{  
  size_t best_move;
  if (edge_exists(game->graph, game->pl_pos[p->id], game->pl_pos[p->opnt_id]))
    {
      size_t shortest_distance = SIZE_MAX;
      struct move_list moves = available_moves(game, p->id);
      for (size_t i = 0; i < moves.n; i++)
	{
	  game->pl_pos[p->id] = moves.l[i];
	  struct queue* path = shortest_path(game, p->id);
	  if (path->length < shortest_distance)
	    {
	      shortest_distance = path->length;
	      best_move = moves.l[i];
	    }
	  queue_free(path);
	}
    }
  
  else
    {
      struct queue* path = shortest_path(game, p->id);
      best_move = queue_peek(path);
      queue_free(path);
    }
  
  game->pl_pos[p->id] = best_move;
  struct move_t move = {best_move, {no_edge(), no_edge()}, MOVE, p->id};
  return move;
}
