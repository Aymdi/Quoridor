#include "test_greedy.h"


static void print_green_ok()
{
  printf("\x1b[32;1mOK\x1b[0m\n");
}

/* Client returns the name "Greedy" */
void test_greedy_get_player_name()
{
  printf("> greedy_get_player_name() - ");
  assert(strcmp(greedy_get_player_name(), "Greedy") == 0);
  print_green_ok();
}

/* Client chooses a correct starting position as BLACK or WHITE */
void test_greedy_first_move()
{
  printf("> greedy_first_move() - ");
  size_t m = 9; // standard board width
  size_t nb_walls = (2*m*(m-1))/15 + ((2*m*(m-1))%15 > 0);

  // client starts first 
  struct graph_t* g1 = graph__initialize(m);
  greedy_initialize(BLACK, g1, nb_walls);
  struct move_t first1 = greedy_first_move();
  assert(first1.m == 4);
  assert(first1.t == MOVE);
  assert(first1.c == BLACK);
  assert(is_no_edge(first1.e[0]));
  assert(is_no_edge(first1.e[1]));
  greedy_finalize();

  // client starts second
  struct graph_t* g2 = graph__initialize(m);
  greedy_initialize(WHITE, g2, nb_walls);
  struct move_t first2 = greedy_first_move();
  assert(first2.m == 76);
  assert(first2.t == MOVE);
  assert(first2.c == WHITE);
  assert(is_no_edge(first2.e[0]));
  assert(is_no_edge(first2.e[1]));
  greedy_finalize();

  print_green_ok();
}

/* Client chooses the move that minimizes its distance to the other side */
void test_greedy_move()
{
  printf("> greedy_move() - ");
  size_t m = 9; // standard board width
  size_t nb_walls = (2*m*(m-1))/15 + ((2*m*(m-1))%15 > 0);
  struct graph_t* g = graph__initialize(m);
  greedy_initialize(BLACK, g, nb_walls);
  greedy_first_move();

  // client moves downward
  struct move_t move1 = greedy_move();
  assert(move1.m == 13);
  assert(move1.t == MOVE);
  assert(move1.c == BLACK);
  assert(is_no_edge(move1.e[0]));
  assert(is_no_edge(move1.e[1]));

  // client moves right to avoid wall
  struct edge_t wall1[2] = {{12, 21}, {13, 22}};
  place_wall(g, wall1);
  struct move_t move2 = greedy_move();
  assert(move2.m == 14);
  assert(move2.t == MOVE);
  assert(move2.c == BLACK);
  assert(is_no_edge(move2.e[0]));
  assert(is_no_edge(move2.e[1]));

  // client reaches the other side
  size_t expected[7] = {23, 32, 41, 50, 59, 68, 77};
  for (size_t i = 0; i < 7; i++)
    {
      struct move_t move_i = greedy_move();
      assert(move_i.m == expected[i]);
    }
  
  greedy_finalize();
  print_green_ok();
}     

/* Multiple shortest paths
 * Clients chooses the one that requires to jump over its opponent (best move)
 */
void test_greedy_play_1()
{
  printf("> greedy_play() 1/2 - ");
  size_t m = 9; // standard board width
  size_t nb_walls = 0;
  struct graph_t* g = graph__initialize(m);
  greedy_initialize(BLACK, g, nb_walls);

  // choose a starting position
  struct move_t prev1 = {SIZE_MAX, {no_edge(), no_edge()}, NO_TYPE, NO_COLOR};
  struct move_t move1 = greedy_play(prev1);
  assert(move1.m == 4);
  assert(move1.t == MOVE);
  assert(move1.c == BLACK);
  assert(is_no_edge(move1.e[0]));
  assert(is_no_edge(move1.e[1]));

  // set up a "jump over opponent" move in a few turns
  struct move_t prev2 = {32, {no_edge(), no_edge()}, MOVE, WHITE};
  struct move_t move2 = greedy_play(prev2);
  assert(move2.m == 13);
  assert(move2.t == MOVE);
  assert(move2.c == BLACK);
  assert(is_no_edge(move2.e[0]));
  assert(is_no_edge(move2.e[1]));

  // wall to the left, doesn't change the shortest path
  struct edge_t wall1[2] = {{30, 31}, {39, 40}};
  struct move_t prev3 = {SIZE_MAX, {wall1[0], wall1[1]}, WALL, NO_COLOR};
  struct move_t move3 = greedy_play(prev3);
  assert(move3.m == 22);
  assert(move3.t == MOVE);
  assert(move3.c == BLACK);

  //another wall to the left that doesn't change the shortest path
  struct edge_t wall2[2] = {{20, 29}, {21, 30}};
  struct move_t prev4 = {SIZE_MAX, {wall2[0], wall2[1]}, WALL, NO_COLOR};
  struct move_t move4 = greedy_play(prev4);
  assert(move4.m == 31);
  assert(move4.t == MOVE);
  assert(move4.c == BLACK);
  
  // multiple shortest paths to the other side
  // clients jumps over opponent (best move)
  struct edge_t wall3[2] = {{40, 49}, {41, 50}};
  struct move_t prev5 = {SIZE_MAX, {wall3[0], wall3[1]}, WALL, NO_COLOR};
  struct move_t move5 = greedy_play(prev5);
  assert(move5.m == 33);
  assert(move5.t == MOVE);
  assert(move5.c == BLACK);

  greedy_finalize();
  print_green_ok();
}  


void test_greedy_wall()
{
  printf("> greedy_wall() - ");
  size_t m = 9; // standard board width
  size_t nb_walls = 10;
  struct graph_t* g = graph__initialize(m);
  struct game_state game;
  game_init(&game, g, 'c', nb_walls);
  greedy_initialize(WHITE, g, nb_walls); // client starts on vertex 76

  game.pl_pos[BLACK] = 5;
  struct move_t prev1 = {5, {no_edge(), no_edge()}, MOVE, BLACK};
  struct move_t move1 = greedy_play(prev1);
  game.pl_pos[WHITE] = 76;
  assert(move1.t == MOVE);
  assert(move1.m == 76);

  struct move_t wall1 = greedy_wall();
  place_wall(game.graph, wall1.e);
  assert(wall1.t == WALL);
  assert(wall1.m == SIZE_MAX);

  assert(wall1.e[0].fr == 5);
  assert(wall1.e[0].to == 14);
  assert(wall1.e[1].fr == 6);
  assert(wall1.e[1].to == 15);

  struct move_t wall2 = greedy_wall();
  place_wall(game.graph, wall2.e);
  assert(wall2.t == WALL);
  assert(wall2.m == SIZE_MAX);
  assert(wall2.e[0].fr == 4);
  assert(wall2.e[0].to == 5);
  assert(wall2.e[1].fr == 13);
  assert(wall2.e[1].to == 14);
  
  struct move_t wall3 = greedy_wall();
  place_wall(game.graph, wall3.e);
  assert(wall3.t == WALL);
  assert(wall3.m == SIZE_MAX);
  assert(wall3.e[0].fr == 15);
  assert(wall3.e[0].to == 24);
  assert(wall3.e[1].fr == 16);
  assert(wall3.e[1].to == 25);

  greedy_finalize();
  print_green_ok();
} 
  

void test_greedy_play_2()
{
  printf("> greedy_play() 2/2 - ");
  size_t m = 9; // standard board width
  size_t nb_walls = 10;
  struct graph_t* g = graph__initialize(m);
  struct game_state game;
  game_init(&game, g, 'c', nb_walls);
  greedy_initialize(BLACK, g, nb_walls); // client starts on vertex 4

  struct move_t no_move = {SIZE_MAX, {no_edge(), no_edge()}, NO_TYPE, NO_COLOR};
  struct move_t first_move = greedy_play(no_move);
  
  game.pl_pos[BLACK] = first_move.m;
  game.pl_pos[WHITE] = 40;
  struct move_t prev = {40, {no_edge(), no_edge()}, MOVE, WHITE};

  while (1)
    {
      struct move_t move = greedy_play(prev);
      if (move.t == MOVE)
	game.pl_pos[BLACK] = move.m;
      else
	place_wall(game.graph, move.e);
      if (gsl_spmatrix_uint_get(g->o, WHITE, game.pl_pos[BLACK]))
	break;
      
      struct queue* path = shortest_path(&game, WHITE);
      prev = (struct move_t){queue_peek(path), {no_edge(), no_edge()}, MOVE, WHITE};
      queue_free(path);
      game.pl_pos[WHITE] = prev.m;
    }
	     
  assert(gsl_spmatrix_uint_get(g->o, WHITE, game.pl_pos[BLACK]));
  assert(gsl_spmatrix_uint_get(g->o, BLACK, game.pl_pos[WHITE]) == 0);
      
  greedy_finalize();
  print_green_ok();
}

void test_greedy()
{
  printf("TESTING: greedy.c\n");
  test_greedy_get_player_name();
  test_greedy_first_move();
  test_greedy_move();
  test_greedy_play_1();
  test_greedy_wall();
  test_greedy_play_2();
  printf("All tests OK\n\n");
}


