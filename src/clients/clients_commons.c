#include "clients_commons.h"


// initialize a client
void client_init(enum color_t id, struct graph_t* g, size_t nb_walls, struct game_state* game, struct player* p)
{
  p->id = id;
  p->opnt_id = (id+1)%2;
  p->walls = malloc(2*sizeof(struct edge_t)*nb_walls);
  game_init(game, g, 'c', nb_walls);
}


// choose the starting position
struct move_t first_move(struct game_state* game, struct player* p)
{
  struct queue* starting_positions = queue_empty();
  for (size_t v = 0; v < game->graph->num_vertices; v++)
    {
      if (matrix_get(game->graph->o, p->id, v))
	queue_enqueue(starting_positions, v);
    }

  struct q_elem* e = starting_positions->front;
  for (size_t v = 0; v < starting_positions->length/2; v++)
    {
      e = e->next;
    }
  size_t start = e->data;
  queue_free(starting_positions);
  game->pl_pos[p->id] = start;
  struct move_t move = {start, {no_edge(), no_edge()}, MOVE, p->id};
  return move;
}


// update the game according to the previous move of the opponent
void update_game(struct game_state* game, struct move_t previous_move)
{
  game->turn++;
  if (previous_move.t == WALL)
    {
      place_wall(game->graph, previous_move.e);
      game->nb_walls[previous_move.c]--;
    }
  if (previous_move.t == MOVE)
    game->pl_pos[previous_move.c] = previous_move.m;
}


// free the memory used by the client
void client_free(struct game_state* game, struct player* p)
{
  graph__free(game->graph);
  free(p->walls);
}
