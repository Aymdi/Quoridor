#ifndef _QUOR_QUEUE_UINT_H_
#define _QUOR_QUEUE_UINT_H_

#include <stddef.h>
#include <stdlib.h>


/* An implementation of a FIFO data structure for size_t elements. It also
 * has LIFO capabilities thanks to the queue_squeeze() function.
 * This structure is involved in the computation of  the shortest path 
 * between two vertices. Its LIFO aspect is used to handle paths (stacks 
 * of vertices).
 */

/* Element of a queue */
struct q_elem {
  size_t data;         // The value of the element
  struct q_elem* next; // The next element in the queue (front to back)
};

/* Queue structure */
struct queue {
  struct q_elem* front; // The first element of the queue
  struct q_elem* back;  // The last element of the queue
  size_t length;        // The length of the queue
};

const struct q_elem sentinel;

#define Q_SENTINEL ((struct q_elem *)&sentinel)


// a predicate telling if an element is a sentinel
int is_sentinel(struct q_elem* e);

// a pointer to a heap-allocated element
struct q_elem* q_element(size_t v);

// an empty queue
struct queue* queue_empty();

// free the memory associated to a queue
void queue_free(struct queue* q);

// a predicate telling if a queue is empty
int queue_is_empty(struct queue* q);
		  
// add an element to the back of the queue
void queue_enqueue(struct queue* q, size_t v);

// add an element to the front of the queue 
void queue_squeeze(struct queue* q, size_t v);

// the value in front of the queue
// PRECOND: queue is not empty
size_t queue_peek(struct queue* q);

// remove the element in front of the queue and return its data
// PRECOND: queue is not empty
size_t queue_dequeue(struct queue* q);

// print the content of a queue
void queue_debug(struct queue* q);

  
#endif // _QUOR_QUEUE_UINT_H_
