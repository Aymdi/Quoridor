#ifndef _QUOR_STRAT_SHATRANJ_H_
#define _QUOR_STRAT_SHATRANJ_H_

#include "clients_commons.h"
#include "shortest_path.h"


/*                             The Shatranj strategy:
 *
 * Named after the top ten Board Game Arena player that uses it,
 * this opening is similar to the Shiller and arguably more powerful.
 * Then, due to the implementation of the shortest_path() function, when there are
 * multiple shortest paths, it will always try to go forward first (instead of 
 * going west or east).
 */

// choose the starting position
struct move_t first_shatranj_move(struct game_state* game, struct player* p);

// a move using the Shatranj strategy
struct move_t shatranj_move(struct game_state* game, struct player* p);

#endif //_QUOR_STRAT_SHATRANJ_H_