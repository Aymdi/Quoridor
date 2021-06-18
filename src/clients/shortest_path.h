#ifndef _QUOR_SHORTEST_PATH_H_
#define _QUOR_SHORTEST_PATH_H_

#include "../rules.h"
#include "../queues/queue_uint.h"


/* An algorithm that returns the (or one of the) shortest path(s) from a 
 * player's position to its opponent's starting zone. This operation is 
 * performed using solely a FIFO data structure. As the weight of every
 * edge is equal, there is no need to use a priority queue.
 */
struct queue* shortest_path(struct game_state* game, enum color_t p);


#endif // _QUOR_SHORTEST_PATH_H_
