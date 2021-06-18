#ifndef _QUOR_STRAT_GREEDY_H
#define _QUOR_STRAT_GREEDY_H

#include "clients_commons.h"
#include "shortest_path.h"


/*                            Greedy strategy:
 *
 * This strategy takes decisions based on the return value of 
 * the greedy_score() function. The score is defined as follows:
 *    score = |shortest_path(opponent)| - |shortest_path(self)|
 * It will move closer to the other side if that score is superior to
 * a certain value. Otherwise, it searches for a wall play that 
 * immediately increases that score. 
 * Wall plays are searched for starting from the first vertex of the graph.  
 * If there are multiple "best" wall plays, the one chosen is the first one
 * that was discovered.
 *
 * NOTE: in case both players start in the same column of vertices, there 
 * aren't any wall plays that can increase the score during the first turn.
 * Consequently, there is no need to try to search for a wall play during
 * the first turns.
 */


/* The length of a player's shortest path to the other side */
size_t distance(struct game_state* game, enum color_t c);

  
/*
 * The score associated to the current situtation on the board
 * score = |shortest_path(opponent)| - |shortest_path(self)|
 */
int greedy_score(struct game_state* game, struct player* p);


/*
 * A move_t of type WALL
 * Try every possible wall, and keep the one that maximizes the value
 * score = |shortest_path(opponent)| - |shortest_path(self)|
 * PRECOND:
 *   - game.nb_walls[self.id] > 0
 * POSTCOND:
 *   - the wall is placed on game.graph
 */
struct move_t greedy_wall(struct game_state* game, struct player* p);



#endif // _QUOR_STRAT_GREEDY_H
