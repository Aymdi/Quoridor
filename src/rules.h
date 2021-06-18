#ifndef _QUOR_RULES_H_
#define _QUOR_RULES_H_

#include "graph_utils.h"
#include "queues/queue_uint.h"


/* Functions related to the rules of the Quoridor game. The game_state
 * structure concentrates all of the required information about a game.
 * The move_list structure serves as storage for the list of available
 * vertices that can be reached a position.
 */


/* Various kinds of info about the current state of a game */
struct game_state {
  struct graph_t* graph; // The graph where the game is played
  char board_shape;      // The shape of the board
  size_t board_width;
  size_t pl_pos[2];      // The positions of both players
  size_t nb_walls[2];    // The number of walls each player has left
  size_t turn;           // The current turn
};

/* List of moves */
struct move_list {
  size_t l[5]; // At most 5 different vertices can be reached from a given vertex
  size_t n;    // Number of moves 
};


/* Initialize a game_state
 * POSTCOND:
 * - game.graph points to the graph_t given in parameters
 * - positions of both players is set to SIZE_MAX
 * - the number of walls that each player has is set to nb_walls
 * - the turn is set to 0
 */
void game_init(struct game_state* game, struct graph_t* g, char shape, size_t nb_walls);


/* A predicate telling if a move (move_t of type MOVE) is allowed
 * A move is allowed if the destination vertex can be reached from 
 * the position of player p, according to the rules of Quoridor.
 */
int move_is_licit(struct game_state* game, enum color_t p, size_t dest);


/* A predicate telling if a player can reach its opponent's starting zone
 * A player can reach that zone if there exists a path from the player's
 * position to one of the vertices of the zone. The existence of such path 
 * is determined by performing a breadth-first search in the game graph.
 */
int path_exists_bfs(struct game_state* game, enum color_t p);


/* A predicate telling if a wall (move_t of type WALL) is allowed
 * A wall is allowed if it blocks two adjacent edges that weren't blocked
 * before, and it doesn't prevent any player from reaching the other side.
 */
int wall_is_licit(struct game_state* game, struct edge_t e[2]);


/* A predicate telling if a play (move_t of type MOVE or WALL) is allowed 
 * A play is allowed if:
 * - it's a wall that is licit, and the player who plays that wall has 
 *   walls left
 * - it's a move that is licit
 */
int play_is_licit(struct game_state* game, struct move_t mv);      


/* The list of moves that are available to a player */
struct move_list available_moves(struct game_state* game, enum color_t p);

#endif // _QUOR_RULES_H_
	  
	  
	  
      
      

      
      
		
      
	    
      
      
