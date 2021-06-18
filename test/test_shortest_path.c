#include "test_shortest_path.h"


static void print_green_ok()
{
  printf("\x1b[32;1mOK\x1b[0m\n");
}

// the last node of a path (= stack of vertices)
size_t follow_path(struct graph_t* g, struct queue* path, size_t src)
{
  size_t v = src;
  struct q_elem* next = path->front;
  while (!is_sentinel(next) && edge_exists(g, v, next->data))
    {
      v = next->data;
      next = next->next;
    }
  return v;
}

void test_shortest_path()
{
  printf("TESTING: shortest_path.c\n");
  printf("> shortest_path() - ");

  struct game_state game;
  size_t m = 9;
  char shape = 'c';
  size_t nb_walls = 10;
  struct graph_t* g = graph__initialize(m);
  game_init(&game, g, shape, nb_walls);
  game.pl_pos[BLACK] = 4; // starting position for player 0
  game.pl_pos[WHITE] = 76; // starting position for player 1

  struct queue* path1 = shortest_path(&game, WHITE);
  assert(path1->length == 8);
  assert(follow_path(game.graph, path1, 76) == 4);
  queue_free(path1);

  struct edge_t wall1[2] = {{66, 67}, {75, 76}};
  assert(wall_is_licit(&game, wall1));
  place_wall(game.graph, wall1);
  struct queue* path2 = shortest_path(&game, WHITE);
  assert(path2->length == 8);
  assert(follow_path(game.graph, path2, 76) == 4);
  queue_free(path2);

  struct edge_t wall2[2] = {{58, 67}, {59, 68}};
  assert(wall_is_licit(&game, wall2));
  place_wall(game.graph, wall2);
  struct queue* path3 = shortest_path(&game, WHITE);
  assert(path3->length == 10);
  assert(follow_path(game.graph, path3, 76) == 6);
  queue_free(path3);

  struct edge_t wall3[2] = {{50, 59}, {51, 60}};
  assert(wall_is_licit(&game, wall3));
  place_wall(game.graph, wall3);
  struct queue* path4 = shortest_path(&game, WHITE);
  assert(path4->length == 11);
  assert(follow_path(game.graph, path4, 76) == 7);
  queue_free(path4);

  struct queue* path5 = shortest_path(&game, BLACK);
  assert(path5->length == 9);
  assert(follow_path(game.graph, path5, 4) == 75);
  queue_free(path5);

  graph__free(g);
  print_green_ok();
  printf("All tests OK\n\n");
}
