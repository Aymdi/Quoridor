#include <dlfcn.h>
#include <time.h>
#include "graph_utils.h"
#include "game_graphs.h"
#include "graph_debug.h"
#include "rules.h"
#include "input.h"


static size_t m = 9;
static char board_shape = 'c';
static int interface = 0;

// dlopen with error handling
void* load_client(char* client_name)
{
  dlerror();
  void* handle = dlopen(client_name, RTLD_NOW);
  if (handle)
    return handle;
  printf("ERROR:\n> Couldn't load %s.\n", client_name);
  printf("  %s\n", dlerror());
  exit(EXIT_FAILURE);
}


// dlsym with error handling
void* load_function(void* client, char* function_name)
{
  dlerror();
  void* function = dlsym(client, function_name);
  char* error = dlerror();
  if (!error)
    return function;
  printf("ERROR:\n> Couldn't load function %s.\n", function_name);
  printf("  %s\n", error);
  exit(EXIT_FAILURE);
}



int main(int argc, char* argv[])
{
  ////////////////////////////////////////////////////////////////////////////
  // Parse options and decide which client plays first
  parse_opts(argc, argv, &m, &board_shape, &interface);
  check_clients(argc, argv);
  char* client[2] = {argv[argc-2], argv[argc-1]};
  srand(time(0));
  enum color_t first_player = rand()%2; // first player
  void* handle[2] = {load_client(client[first_player]),
		     load_client(client[(first_player+1)%2])};


  ////////////////////////////////////////////////////////////////////////////
  // Load functions from clients
  const char* (*name[2])();
  name[0] = load_function(handle[0], "get_player_name");
  name[1] = load_function(handle[1], "get_player_name");

  void (*initialize[2])(enum color_t id, struct graph_t* g, size_t nb_wall);
  initialize[0] = load_function(handle[0], "initialize");
  initialize[1] = load_function(handle[1], "initialize");

  struct move_t (*play[2])(struct move_t previous_move);
  play[0] = load_function(handle[0], "play");
  play[1] = load_function(handle[1], "play");
  
  void (*finalize[2])();
  finalize[0] = load_function(handle[0], "finalize");
  finalize[1] = load_function(handle[1], "finalize");

  
  ////////////////////////////////////////////////////////////////////////////
  // Compute how many walls players receive
  printf("%s VS %s", name[0](), name[1]());
  printf(" (%s starts)\n", name[0]());
  size_t nb_walls = (2*m*(m-1))/15 + ((2*m*(m-1))%15 > 0);
  printf("Players receive %zu walls.\n", nb_walls);


  ////////////////////////////////////////////////////////////////////////////
  // Initialize variables required to play the game
  struct move_t move = {-1, {{-1,-1},{-1,-1}}, NO_TYPE, NO_COLOR};
  struct graph_t* g = generating__graph(board_shape, m);
  //  struct graph_t* g = graph__initialize(m);
  struct game_state game;
  game_init(&game, g, board_shape, nb_walls);
  struct graph_t* player_graphs[2] = {graph__copy(g), graph__copy(g)};

  
  ////////////////////////////////////////////////////////////////////////////
  // Start
  printf("START\n");
  initialize[0](BLACK, player_graphs[0], nb_walls);
  initialize[1](WHITE, player_graphs[1], nb_walls);
  
  enum color_t p = 0;
  while (1)
    {
      game.turn++;
      move = play[p](move);
      if (move.c != p || !play_is_licit(&game, move))
	{
	  printf("ERROR: player %d (%s) sent a move that is not accepted:", p, name[p]());
	  printf(" {%zu, {{%zu, %zu}, {%zu, %zu}}, %s, %s}\n",
		 move.m,
		 move.e[0].fr, move.e[0].to, move.e[1].fr, move.e[1].to,
		 (move.t == WALL) ? "WALL" : "MOVE",
		 (move.c == BLACK) ? "BLACK" : "WHITE");
	  exit(EXIT_FAILURE);
	}
      
      if (move.t == MOVE)
	{
	  game.pl_pos[p] = move.m;
	  if (gsl_spmatrix_uint_get(g->o, (p+1)%2, move.m)) // win
	    break;
	}
      
      if (move.t == WALL)
	{
	  place_wall(g, move.e);
	  game.nb_walls[move.c]--;
	}
      
      if (interface)
	{
	  printf("Turn %zu\n", game.turn);
	  display_graph(g, game.pl_pos);
	  printf("\n");
	}
      
      p = (p+1)%2;
    }
  printf("Player %d (%s) wins !\n", p, name[p]());
  if (interface)
    display_graph(g, game.pl_pos);

  
  ////////////////////////////////////////////////////////////////////////////
  // Memory clean-up
  finalize[0]();
  finalize[1]();
  graph__free(g);

  dlclose(handle[0]);
  dlclose(handle[1]);
  
  return 0;
}
