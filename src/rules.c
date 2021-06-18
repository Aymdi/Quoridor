#include "rules.h"
#include <stdio.h>


// initialize a game
void game_init(struct game_state* game, struct graph_t* g, char shape, size_t nb_walls)
{
  game->graph = g;
  game->board_shape = shape;
  game->board_width = (size_t) sqrt(g->num_vertices);
  game->pl_pos[BLACK] = SIZE_MAX;
  game->pl_pos[WHITE] = SIZE_MAX;  
  game->nb_walls[0] = nb_walls;
  game->nb_walls[1] = nb_walls;
  game->turn = 0;
} 
  

// a predicate telling if a move is allowed
// PRECOND: game->pl_pos[(p+1)%2] < game->graph->num_vertices and
//          dest < game->graph->num_vertices 
int move_is_licit(struct game_state* game, enum color_t p, size_t dest)
{
  size_t source = game->pl_pos[p];
  if (source == SIZE_MAX)
    return matrix_get(game->graph->o, p, dest);
  size_t opnt_pos = game->pl_pos[(p+1)%2];
  
  // moving up, down, left or right
  if (edge_exists(game->graph, source, dest)) 
    return (opnt_pos != dest);

  unsigned int dir = matrix_get(game->graph->t, source, opnt_pos);
  if (dir == 0) // opponent isn't located on a neighbor vertex 
    return 0;

  // jumping over opponent with a wall or border behind
  if (vertex_neighbor(game->graph, opnt_pos, dir) == SIZE_MAX)       
    return (edge_exists(game->graph, opnt_pos, dest)
	    && dest != source);
  else
    return (dest == vertex_neighbor(game->graph, opnt_pos, dir));
}


// a predicate telling if a player can reach its opponent's starting zone
int path_exists_bfs(struct game_state* game, enum color_t p)
{
  size_t n = game->graph->num_vertices;
  size_t m = game->board_width;
  int* enqueued = calloc(n, sizeof(int)); // keep track of vertices in queue
  struct queue* q = queue_empty();
  int reached = 0;
  size_t v = game->pl_pos[p];
  
  queue_enqueue(q, v);
  enqueued[v] = 1;
  while (!queue_is_empty(q) && !reached)
    {
      v = queue_dequeue(q);
      if (matrix_get(game->graph->o, (p+1)%2, v)) // zone reached
	reached = 1;
      size_t neighbors[4] = {v-m, v+m, v-1, v+1};
      for (size_t i = 0; i < 4; i++)
	{
	  if (neighbors[i] < n && edge_exists(game->graph, v, neighbors[i]) && !enqueued[neighbors[i]])
	    {
	      queue_enqueue(q, neighbors[i]);
	      enqueued[neighbors[i]] = 1;
	    }
	}
    }

  free(enqueued);
  queue_free(q);
  return reached;
} 


// a predicate telling if a wall is allowed
int wall_is_licit(struct game_state* game, struct edge_t e[2])
{
  // distinguish two cases: either both edges have the same orientation or they don't
  
  size_t way0 = matrix_get(game->graph->t, e[0].fr, e[0].to);
  size_t way1 = matrix_get(game->graph->t, e[1].fr, e[1].to);
  if (way0 != way1) // rectify orientation of e[1]
    {
      size_t temp = e[1].fr;
      e[1].fr = e[1].to;
      e[1].to = temp;
    }
  if (edge_exists(game->graph, e[0].fr, e[0].to)
      && edge_exists(game->graph, e[1].fr, e[1].to)
      && (edge_exists(game->graph, e[0].fr, e[1].fr)
	  || edge_exists(game->graph, e[0].to, e[1].to)))
    {
      place_wall(game->graph, e);
      int licit = 1;
      if (!path_exists_bfs(game, BLACK) || !path_exists_bfs(game, WHITE))
	licit = 0;
      remove_wall(game->graph, e);
      return licit;
    }
  return 0;
}  


// a predicate telling if a play (move or wall) is allowed
int play_is_licit(struct game_state* game, struct move_t move)
{
  if (move.t == WALL && game->nb_walls[move.c])
    return wall_is_licit(game, move.e);
  if (move.t == MOVE && move.m < game->graph->num_vertices)
    return move_is_licit(game, move.c, move.m);
  return 0;
}


// moves available to a player 
struct move_list available_moves(struct game_state* game, enum color_t p)
{
  struct move_list list;
  list.n = 0;
  size_t m = game->board_width, n = game->graph->num_vertices;
  size_t source = game->pl_pos[p], opnt_pos = game->pl_pos[(p+1)%2];
  
  size_t neighbors[4] = {source-m, source+m, source-1, source+1};
  for (size_t v = 0; v < 4; v++)
    {
      if (neighbors[v] < n && move_is_licit(game, p, neighbors[v]))
	{
	  list.l[list.n] = neighbors[v];
	  list.n++;
	}
    }
  // check for eventual jumps over opponent
  if (edge_exists(game->graph, source, opnt_pos))
    {
      size_t jumps[4] = {opnt_pos-m, opnt_pos+m, opnt_pos-1, opnt_pos+1};
      for (size_t v = 0; v < 4; v++)
	{
	  if (jumps[v] < n && move_is_licit(game, p, jumps[v]))
	    {
	      list.l[list.n] = jumps[v];
	      list.n++;
	    }
	}
    }
  return list;
}
