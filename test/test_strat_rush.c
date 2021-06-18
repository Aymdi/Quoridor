#include "test_strat_rush.h"


static void print_green_ok()
{
  printf("\x1b[32;1mOK\x1b[0m\n");
}


void test_rush_move()
{
  printf("> rush_move() - ");
  size_t m = 9; // standard board width
  size_t nb_walls = 10;
  struct game_state game;
  struct player p;
  struct graph_t* g = graph__initialize(m);
  client_init(BLACK, g, nb_walls, &game, &p);
  game.pl_pos[BLACK] = 4;
  
  // client moves downward
  struct move_t move1 = rush_move(&game, &p);
  assert(move1.m == 13);
  assert(move1.t == MOVE);
  assert(move1.c == BLACK);
  assert(is_no_edge(move1.e[0]));
  assert(is_no_edge(move1.e[1]));

  // client moves right to avoid wall
  struct edge_t wall1[2] = {{12, 21}, {13, 22}};
  place_wall(g, wall1);
  struct move_t move2 = rush_move(&game, &p);
  assert(move2.m == 14);
  assert(move2.t == MOVE);
  assert(move2.c == BLACK);
  assert(is_no_edge(move2.e[0]));
  assert(is_no_edge(move2.e[1]));

  game.pl_pos[WHITE] = 59;

  // client reaches the other side
  size_t expected[6] = {23, 32, 41, 50, 68, 77};
  for (size_t i = 0; i < 6; i++)
    {
      struct move_t move_i = rush_move(&game, &p);
      assert(move_i.m == expected[i]);
    }
  
  client_free(&game, &p);
  print_green_ok();
}


void test_strat_rush()
{
  printf("TESTING: strat_rush.c\n");
  test_rush_move();
  printf("All tests OK\n\n");
}
