#include "queue_uint.h"
#include <stdio.h>

// a predicate telling if an element is a sentinel
int is_sentinel(struct q_elem* e)
{
  return (e == Q_SENTINEL);
}


// a pointer to a heap-allocated element
struct q_elem* q_element(size_t v)
{
  struct q_elem* e = malloc(sizeof(struct q_elem));
  e->data = v;
  e->next = Q_SENTINEL;
  return e;
}


// an empty queue
struct queue* queue_empty()
{
  struct queue* q = malloc(sizeof(struct queue));
  q->front = Q_SENTINEL;
  q->back = Q_SENTINEL;
  q->length = 0;
  return q;
}


// free the memory associated to a queue
void queue_free(struct queue* q)
{
  struct q_elem* current = q->front;
  while (!is_sentinel(current))
    {
      struct q_elem* new = current->next;
      free(current);
      current = new;
    }
  free(q);
}
	  

// a predicate telling if a queue is empty
int queue_is_empty(struct queue* q)
{
  return (is_sentinel(q->front) && is_sentinel(q->back) && q->length == 0);
}


// add an element to the back of the queue
void queue_enqueue(struct queue* q, size_t v)
{
  struct q_elem* e = q_element(v);
  if (queue_is_empty(q))
    q->front = e;
  else
    q->back->next = e;
  q->back = e;
  q->length++;
}


// add an element to the front of the queue 
void queue_squeeze(struct queue* q, size_t v)
{
  struct q_elem* e = q_element(v);
  if (queue_is_empty(q))
    q->back = e;
  else
    e->next = q->front;
  q->front = e;
  q->length++;
}


// the value in front of the queue
// PRECOND: queue is not empty
size_t queue_peek(struct queue* q)
{
  return q->front->data;
}


// remove the element in front of the queue and return its data
// PRECOND: queue is not empty
size_t queue_dequeue(struct queue* q)
{
  size_t v = q->front->data;
  if (q->front == q->back) // q has only 1 element
    {
      free(q->front);
      q->front = Q_SENTINEL;
      q->back = Q_SENTINEL;
    }
  else
    {
      struct q_elem* new_front = q->front->next;
      free(q->front);
      q->front = new_front;
    }
  q->length--;
  return v;
}


// print the content of a queue
void queue_debug(struct queue* q)
{
  printf("q = [");
  if (!queue_is_empty(q))
    {
      struct q_elem* e = q->front;
      printf("%zu", e->data);
      e = e->next;
      while (!is_sentinel(e))
	{
	  printf(", %zu", e->data);
	  e = e->next;
	}
    }
  printf("]\n");
}
