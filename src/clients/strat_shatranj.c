#include "strat_shatranj.h"

struct move_t first_shatranj_move(struct game_state* game, struct player* p){
  size_t start;
  if (p->id == BLACK)
    start = game->board_width/2;
  else
    start = game->graph->num_vertices-1-game->board_width/2;
  game->pl_pos[p->id] = start;
  struct move_t move = {start, {no_edge(), no_edge()}, MOVE, p->id};
  return move; 
}

struct move_t shatranj_move(struct game_state* game, struct player* p){
  size_t best_move;
  if ( (game->turn==0) || (game->turn==1) ){
      struct move_t move = {SIZE_MAX, {(struct edge_t){game->pl_pos[p->id], game->pl_pos[p->id]-1},(struct edge_t) {game->pl_pos[p->id]+game->board_width,game->pl_pos[p->id]+game->board_width-1 } }, WALL, p->id};
      return move;
  }
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