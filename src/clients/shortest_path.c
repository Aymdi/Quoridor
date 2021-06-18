#include "shortest_path.h"


struct queue* shortest_path(struct game_state* game, enum color_t p)
{
  /* Same principle as path_exists_bfs, except enqueued (called distance here)
   * also keeps track of distances to source vertex
   */
  size_t n = game->graph->num_vertices;
  size_t m = game->board_width;
  
  struct queue* q = queue_empty();
  size_t v = game->pl_pos[p];
  int reached = matrix_get(game->graph->o, (p+1)%2, v);

  /* Checking if player is already in the winning zone */
  if (reached){
    return q;
  }
  
  int* distance = calloc(n, sizeof(int));
  
  queue_enqueue(q, v);
  distance[v] = 0;
  while (!queue_is_empty(q) && !reached)
    {
      v = queue_dequeue(q);
      if (matrix_get(game->graph->o, (p+1)%2, v)) // zone reached
	reached = 1;
      size_t neighbors[4] = {v-m, v+m, v-1, v+1};
      for (size_t i = 0; i < 4; i++)
	{
	  if (neighbors[i] < n && edge_exists(game->graph, v, neighbors[i]) && distance[neighbors[i]] == 0)
	    {
	      queue_enqueue(q, neighbors[i]);
	      distance[neighbors[i]] = distance[v]+1; // keep track of distance
	    }
	}
    }
  queue_free(q);
  /* What's left to do is to backtrack from v (the first vertex in the 
   * winning zone that was reached)
   * Here, backtracking consists in choosing an adjacent vertex v2 such that
   * distance[v2] < distance[v], eventually ending up back at the source vertex
   */
  struct queue* path = queue_empty();
  queue_squeeze(path, v);
  while (distance[v] > 1)
    {
      size_t neighbors[4] = {v-1, v+1, v-m, v+m};
      size_t i = 0;
      while (!(neighbors[i] < n && edge_exists(game->graph, v, neighbors[i]) && distance[neighbors[i]] < distance[v]))
	i++;
      queue_squeeze(path, neighbors[i]);
      v = neighbors[i];
    }
  free(distance);
  return path;
}
  
	  
  

  
