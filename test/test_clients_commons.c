#include "test_clients_commons.h"


static void print_green_ok()
{
  printf("\x1b[32;1mOK\x1b[0m\n");
}


void test_client_init()
{
  printf("> client_init() - ");
  enum color_t id = BLACK;
  size_t m = 9;
  size_t nb_walls = 10;
  struct graph_t* g = graph__initialize(m);
  struct game_state game;
  struct player p;
  client_init(id, g, nb_walls, &game, &p);
  assert(p.id == BLACK);
  assert(p.opnt_id == WHITE);
  
  assert(game.graph == g);
  assert(game.pl_pos[BLACK] == SIZE_MAX);
  assert(game.pl_pos[WHITE] == SIZE_MAX);
  assert(game.nb_walls[BLACK] == 10);
  assert(game.nb_walls[WHITE] == 10);

  client_free(&game, &p);
  print_green_ok();
}


void test_first_move()
{
  printf("> first_move() - ");
  struct graph_t* g = graph__initialize(9);
  struct game_state game;
  struct player p;
  client_init(BLACK, g, 10, &game, &p);
  
  p.id = BLACK;
  struct move_t move1 = first_move(&game, &p);
  assert(move1.m == 4);
  assert(game.pl_pos[BLACK] == 4);

  p.id = WHITE;
  struct move_t move2 = first_move(&game, &p);
  assert(move2.m == 76);
  assert(game.pl_pos[WHITE] == 76);

  client_free(&game, &p);
  print_green_ok();
}


void test_update_game()
{
  printf("> update_game() - ");
  size_t m = 9;
  size_t nb_walls = 10;
  struct graph_t* g = graph__initialize(m);
  struct game_state game;
  game_init(&game, g, 'c', nb_walls);

  struct move_t move1 = {5, {no_edge(), no_edge()}, MOVE, BLACK};
  update_game(&game, move1);
  assert(game.pl_pos[BLACK] == 5);
  assert(game.nb_walls[BLACK] == 10);

  struct edge_t wall1[2] = {{0,1}, {9, 10}};
  struct move_t move2 = {SIZE_MAX, {wall1[0], wall1[1]}, WALL, WHITE};
  update_game(&game, move2);
  assert(game.pl_pos[BLACK] == 5);
  assert(game.pl_pos[WHITE] == SIZE_MAX);
  assert(game.nb_walls[BLACK] == 10);
  assert(game.nb_walls[WHITE] == 9);


  graph__free(g);
  print_green_ok();
}
  

void test_clients_commons()
{
  printf("TESTING: clients_commons.c\n");
  test_client_init();
  test_first_move();
  test_update_game();
  printf("All tests OK\n\n");
}
