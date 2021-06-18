#include "test_rules.h"


static void print_green_ok()
{
  printf("\x1b[32;1mOK\x1b[0m\n");
}


static struct game_state game;
static size_t m = 9;
static char shape = 'c';
static size_t nb_walls = 10;


void test_game_init()
{
  printf("> game_init() - ");
  struct graph_t* g = graph__initialize(m);
  game_init(&game, g, shape, nb_walls);
  assert(game.graph == g);
  assert(game.board_shape == shape);
  assert(game.board_width == m);
  assert(game.pl_pos[BLACK] == SIZE_MAX);
  assert(game.pl_pos[WHITE] == SIZE_MAX);
  assert(game.nb_walls[BLACK] == nb_walls);
  assert(game.nb_walls[WHITE] == nb_walls);
  assert(game.turn == 0);
  
  graph__free(g);
  print_green_ok();
}


void test_move_is_licit()
{
  printf("> move_is_licit() - ");
  struct graph_t* g = graph__initialize(m);
  game_init(&game, g, shape, nb_walls);
  game.pl_pos[BLACK] = 75; // starting position for player 1
  game.pl_pos[WHITE] = 4; // starting position for player 2
  assert(move_is_licit(&game, BLACK, 76)); // move west
  assert(move_is_licit(&game, BLACK, 74)); // move east
  assert(move_is_licit(&game, BLACK, 66)); // move north
  assert(move_is_licit(&game, BLACK, 77) == 0); // two steps east
  assert(move_is_licit(&game, BLACK, 75) == 0); // no move

  game.pl_pos[BLACK] = 13; // player 1 just below player 2
  assert(move_is_licit(&game, BLACK, 4) == 0); // move to an occupied vertex
  assert(move_is_licit(&game, BLACK, 22)); // move down
  assert(move_is_licit(&game, BLACK, 3)); // jump over player 2 and move west

  assert(move_is_licit(&game, WHITE, 22)); // player 2 jumps over player 1
  assert(move_is_licit(&game, WHITE, 12) == 0); // jump over and move west
  assert(move_is_licit(&game, WHITE, 14) == 0); // jump over and move east
  assert(move_is_licit(&game, WHITE, 4) == 0); // no move
  assert(move_is_licit(&game, WHITE, 3)); // move west
  assert(move_is_licit(&game, WHITE, 5)); // move east
  assert(move_is_licit(&game, WHITE, 13) == 0); // move south
  assert(move_is_licit(&game, WHITE, 63) == 0); // move too far away

  disconnect_vertices(game.graph, 4, 5);
  assert(move_is_licit(&game, BLACK, 5) == 0); // jump over player 2 and a wall
  assert(move_is_licit(&game, BLACK, 75) == 0); // move too far away
  
  graph__free(g);
  print_green_ok();
}


void test_path_exists_bfs()
{
  printf("> path_exists_bfs() - ");
  struct graph_t* g = graph__initialize(m);
  game_init(&game, g, shape, nb_walls);
  game.pl_pos[WHITE] = 75;
  game.pl_pos[BLACK] = 4;
  assert(path_exists_bfs(&game, WHITE));
  assert(path_exists_bfs(&game, BLACK));

  disconnect_vertices(game.graph, 4, 13);
  assert(path_exists_bfs(&game, BLACK));
  disconnect_vertices(game.graph, 4, 3);
  assert(path_exists_bfs(&game, BLACK));
  disconnect_vertices(game.graph, 4, 5);
  assert(path_exists_bfs(&game, BLACK) == 0);
  
  for (size_t v = 0; v < m-1; v++)
    {
      disconnect_vertices(game.graph, v, v+m);
    }
  assert(path_exists_bfs(&game, WHITE));
  disconnect_vertices(game.graph, m-1, m-1+m);
  assert(path_exists_bfs(&game, WHITE) == 0);

  graph__free(g);
  print_green_ok();
}


void test_wall_is_licit()
{
  printf("> wall_is_licit() - ");
  struct graph_t* g = graph__initialize(m);
  game_init(&game, g, shape, nb_walls);
  game.pl_pos[BLACK] = 4;
  game.pl_pos[WHITE] = 75;
  
  struct edge_t wall1[2] = {{3,4}, {13,12}}; // vertical wall next to player 0
  assert(wall_is_licit(&game, wall1));
  place_wall(game.graph, wall1);
  assert(edge_exists(game.graph, 3, 4) == 0);
  assert(edge_exists(game.graph, 13, 12) == 0);

  struct edge_t wall2[2] = {{3, 12}, {4, 13}}; // overlaps wall1
  assert(wall_is_licit(&game, wall2) == 0);
  assert(edge_exists(game.graph, 3, 4) == 0);
  assert(edge_exists(game.graph, 12, 13) == 0);

  struct edge_t wall3[2] = {{12, 21}, {13, 22}}; // horizontal wall below wall1
  assert(edge_exists(game.graph, 12, 21));
  assert(edge_exists(game.graph, 13, 22));
  assert(edge_exists(game.graph, 12, 13) == 0);
  assert(edge_exists(game.graph, 21, 22));	
  assert(wall_is_licit(&game, wall3));
  place_wall(game.graph, wall3);
  assert(edge_exists(game.graph, 12, 21) == 0);
  assert(edge_exists(game.graph, 13, 22) == 0);

  struct edge_t wall4[2] = {{4, 5}, {13, 14}}; // locks up player 2
  assert(edge_exists(game.graph, 4, 5));
  assert(edge_exists(game.graph, 13, 14));
  assert(wall_is_licit(&game, wall4) == 0);
  
  graph__free(g);
  print_green_ok();
}


void test_available_moves()
{
  printf("> available_moves() - ");
  struct graph_t* g = graph__initialize(m);
  game_init(&game, g, shape, nb_walls);
  
  game.pl_pos[BLACK] = 0;
  game.pl_pos[WHITE] = 2;
  struct move_list moves1 = available_moves(&game, BLACK);
  assert(moves1.n == 2);
  assert(moves1.l[0] == 9);
  assert(moves1.l[1] == 1);

  disconnect_vertices(g, 0, 1);
  struct move_list moves2 = available_moves(&game, BLACK);
  assert(moves2.n == 1);
  assert(moves2.l[0] == 9);

  game.pl_pos[BLACK] = 10;
  game.pl_pos[WHITE] = 1;
  struct move_list moves3 = available_moves(&game, BLACK);
  assert(moves3.n == 4);
  assert(moves3.l[0] == 19);
  assert(moves3.l[1] == 9);
  assert(moves3.l[2] == 11);
  assert(moves3.l[3] == 2);

  struct move_list moves4 = available_moves(&game, WHITE);
  assert(moves4.n == 2);
  assert(moves4.l[0] == 2);
  assert(moves4.l[1] == 19);

  disconnect_vertices(g, 10, 19);
  struct move_list moves5 = available_moves(&game, WHITE);
  assert(moves5.n == 3);
  assert(moves5.l[0] == 2);
  assert(moves5.l[1] == 9);
  assert(moves5.l[2] == 11);
  
  graph__free(g);
  print_green_ok();
}


void test_rules()
{
  printf("TESTING: rules.c\n");
  test_game_init();
  test_move_is_licit();
  test_path_exists_bfs();
  test_wall_is_licit();
  test_available_moves();
  printf("All tests OK\n\n");
}
