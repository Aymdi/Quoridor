#ifndef _QUOR_STRAT_RUSH_H_
#define _QUOR_STRAT_RUSH_H_

#include "clients_commons.h"
#include "shortest_path.h"


/*                             Rush strategy:
 *
 * This strategy discards playing any wall and focuses on systematically 
 * moving closer to the opponent's starting zone. It doesn't take into
 * account what the opponent's next play could be.
 * Due to the implementation of the shortest_path() function, when there are
 * multiple shortest paths, it will always try to go forward first (instead of 
 * going west or east).
 */

/* A move that shortens a player's shortest path to the other side */
struct move_t rush_move(struct game_state* game, struct player* p);

#endif // _QUOR_STRAT_RUSH_H_
