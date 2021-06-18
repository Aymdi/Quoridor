#include "test_queue_uint.h"


static void print_green_ok()
{
  printf("\x1b[32;1mOK\x1b[0m\n");
}


void test_is_sentinel()
{
  printf("> is_sentinel() - ");
  struct q_elem e;
  assert(is_sentinel(&e) == 0);
  assert(is_sentinel(Q_SENTINEL));
  print_green_ok();
}


void test_q_element()
{
  printf("> q_element() - ");
  struct q_elem* e = q_element(18);
  size_t data = e->data;
  assert(data == 18);
  assert(is_sentinel(e->next));
  free(e);
  print_green_ok();
}
  

void test_queue_empty()
{
  printf("> queue_empty() - ");
  struct queue* q = queue_empty();
  assert(q->length == 0);
  assert(is_sentinel(q->front));
  assert(is_sentinel(q->back));
  free(q);
  print_green_ok();
}


void test_queue_is_empty()
{
  printf("> queue_is_empty() - ");
  struct queue* q = queue_empty();
  assert(queue_is_empty(q));
  q->length = 1;
  assert(queue_is_empty(q) == 0);
  free(q);
  print_green_ok();
}


void test_queue_enqueue()
{
  printf("> queue_enqueue() - ");
  struct queue* q = queue_empty();

  queue_enqueue(q, 1);
  assert(q->back->data == 1);
  assert(q->front->data == 1);
  assert(q->length == 1);
  
  queue_enqueue(q, 3);
  assert((q->back->data) == 3);
  assert(q->length == 2);
  
  queue_enqueue(q, 5);
  assert(q->back->data == 5);
  assert(q->length == 3);

  assert(q->front->data == 1);
  assert(q->front->next->data == 3);
  assert(q->front->next->next->data == 5);
  assert(is_sentinel(q->back->next));
  
  queue_free(q);
  print_green_ok();
}


void test_queue_squeeze()
{
  printf("> queue_squeeze() - ");
  struct queue* q = queue_empty();
  queue_squeeze(q, 1);
  assert(queue_peek(q) == 1);
  assert(q->length == 1);

  queue_squeeze(q, 3);
  assert(queue_peek(q) == 3);
  assert(q->length == 2);

  queue_squeeze(q, 5);
  assert(queue_peek(q) == 5);
  assert(q->length == 3);

  assert(q->front->data == 5);
  assert(q->front->next->data == 3);
  assert(q->front->next->next->data == 1);
  assert(is_sentinel(q->back->next));

  queue_free(q);
  print_green_ok();
}


void test_queue_peek()
{
  printf("> queue_peek() - ");
  struct queue* q = queue_empty();
  queue_enqueue(q, 1);
  assert(queue_peek(q) == 1);
  queue_enqueue(q, 3);
  assert(queue_peek(q) == 1);

  queue_free(q);
  print_green_ok();
}


void test_queue_dequeue()
{
  printf("> queue_dequeue() - ");
  struct queue* q = queue_empty();
  queue_enqueue(q, 1);
  assert(queue_dequeue(q) == 1);
  assert(queue_is_empty(q));

  queue_enqueue(q, 1);
  queue_enqueue(q, 3);
  assert(q->length == 2);
  
  assert(queue_dequeue(q) == 1);
  assert(q->length == 1);
  assert(queue_peek(q) == 3);
  assert(q->back->data == 3);
  
  assert(queue_dequeue(q) == 3);
  assert(q->length == 0);

  queue_free(q);
  print_green_ok();
}


void test_queue_uint()
{
  printf("TESTING: queue_uint.c\n");
  test_is_sentinel();
  test_q_element();
  test_queue_empty();
  test_queue_is_empty();
  test_queue_enqueue();
  test_queue_squeeze();
  test_queue_peek();
  test_queue_dequeue();
  printf("All tests OK\n\n");
}
