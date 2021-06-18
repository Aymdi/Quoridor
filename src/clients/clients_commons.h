#ifndef _CLIENTS_COMMONS_H_
#define _CLIENTS_COMMONS_H_

#include "../graph_debug.h"
#include "../player.h"
#include "../rules.h"


/* Functions that are common between all clients. */


/* A common player structure for all clients */
struct player {
  enum color_t id;      // The color assigned to the client
  enum color_t opnt_id; // The color assigned to the opponent
  struct edge_t* walls; // A pointer to an array of walls, for storage
};


/* Initialize a client
 * POSTCOND:
 * - game->pl_pos[] == {SIZE_MAX, SIZE_MAX}
 * - game->nb_walls[] == {nb_walls, nb_walls}
 * - p->id = id
 * - p->opnt_id = (id+1)%2
 * - p->walls is a pointer to a heap-allocated array with just enough memory 
 *   for nb_walls walls
 */
void client_init(enum color_t id, struct graph_t* g, size_t nb_walls, struct game_state* game, struct player* p);


/* Determine a starting position for a player
 * POSTCOND:
 * - game->pl_pos[p->id] is equal to the position chosen
 */
struct move_t first_move(struct game_state* g, struct player* p);


/* Update a game_state
 * POSTCOND:
 * - if previous_move.t == WALL, the vertices involved with the wall
 *   are now disconnected in game->graph
 * - if previous_move.t == MOVE, game->pl_pos[previous_move.c] is set
 *   to previous_move.m
 */
void update_game(struct game_state* game, struct move_t previous_move);


/* Free the memory used by a client*/
void client_free(struct game_state* game, struct player* p);


#endif // _CLIENTS_COMMONS_H_
