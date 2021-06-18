#include "strat_greedy.h"



size_t distance(struct game_state* game, enum color_t c)
{
  struct queue* path = shortest_path(game, c);
  size_t d = path->length;
  queue_free(path);
  return d;
}


int greedy_score(struct game_state* game, struct player* p)
{
  return (distance(game, p->opnt_id) - distance(game, p->id));
}


struct move_t greedy_wall(struct game_state* game, struct player* p)
{
  size_t m = game->board_width;
  size_t n = game->graph->num_vertices;
  int score_max = greedy_score(game, p); // keep track of the best distance advantage
  int found_best_wall = 0;
  struct edge_t* w = p->walls+2*(game->nb_walls[p->id]-1); // store the best wall
  
  for (size_t v = 0; v < n; v++)
    {
      if (v%m < m-1 && v < n-m-1) 
	{
	  struct edge_t walls[2][2] = {{{v, v+1}, {v+m, v+m+1}},
				       {{v, v+m}, {v+1, v+m+1}}};
	  for (size_t i = 0; i < 2; i++)
	    {
	      if (wall_is_licit(game, walls[i]))
		{
		  place_wall(game->graph, walls[i]);
		  int score = greedy_score(game, p);
		  if (score > score_max)
		    {
		      found_best_wall = 1;
		      score_max = score;
		      w[0] = walls[i][0];
		      w[1] = walls[i][1];
		    }
		  remove_wall(game->graph, walls[i]);
		}
	    }
	}
    }
  
  if (!found_best_wall)
    return (struct move_t) {SIZE_MAX, {no_edge(), no_edge()}, NO_TYPE, NO_COLOR};
  place_wall(game->graph, w);
  game->nb_walls[p->id]--;
  struct move_t move = {SIZE_MAX, {w[0], w[1]}, WALL, p->id};
  return move;
}
