#include "clients_commons.h"
#include <stdio.h>
#include <string.h>

#define MAX_INPUT_LENGTH 64


static char name[MAX_INPUT_LENGTH] = {0};
static struct player self;
static struct game_state game;


/* Memory safe fgets
 * Saves the first n characters given as input in str */
void get_input(char* str, size_t n)
{
  fgets(str, n, stdin);
  // remove the unwanted '\n' at the end
  char* ptr = str;
  while (*ptr != '\0' && *ptr != '\n')
    ptr++;
  if (*ptr == '\n')
    *ptr = '\0';
}


// Ask user to input their name
char const* get_player_name()
{
  if (name[0] == 0)
    {
      printf("Please enter your name:\n$ ");
      get_input(name, MAX_INPUT_LENGTH);
    }      
  return name;
}


void initialize(enum color_t id, struct graph_t* graph, size_t num_walls)
{
  client_init(id, graph, num_walls, &game, &self);
}


/* Process move input from player
 * Recognized inputs:
 * - 4: a move to vertex 4
 * - 13v: a vertical wall to the right of vertex 13
 * - 58h: a horizontal wall below vertex 58
 */ 
struct move_t move_from_input(char* str)
{
  size_t n = game.graph->num_vertices;
  size_t m = game.board_width;

  struct move_t move = {SIZE_MAX, {no_edge(), no_edge()}, NO_TYPE, self.id};
  size_t vertex = 0;
  char orientation = 0;

  while (*str && *str == ' ')
    str++;
  while (*str >= '0' && *str <= '9')
    {
      vertex = 10*vertex + (size_t)(*str - '0');
      str++;
    }
  if (vertex < n)
    move.m = vertex;
  else
    {
      printf("(Out of bounds vertex) ");
      return move;
    }
  
  while (*str && *str == ' ')
    str++;
  if (*str == 'h' || *str == 'H' || *str == 'v' || *str == 'V')
    orientation = *str;
      
  if (vertex%m < m-1 && vertex < n-m-1 && orientation)
    {
      if (orientation == 'h' || orientation == 'H')
	{
	  move.t = WALL;
	  move.e[0] = (struct edge_t){vertex, vertex+m};
	  move.e[1] = (struct edge_t){vertex+1, vertex+m+1};
	}
      if (orientation == 'v' || orientation == 'V')
	{
	  move.t = WALL;
	  move.e[0] = (struct edge_t){vertex, vertex+1};
	  move.e[1] = (struct edge_t){vertex+m, vertex+m+1};
	}
    }
  
  if (!orientation)
    move.t = MOVE;

  return move;
}


struct move_t play(struct move_t previous_move)
{
  update_game(&game, previous_move);
  char input[MAX_INPUT_LENGTH];
  struct move_t move;
  move.t = NO_TYPE;

  while (!play_is_licit(&game, move))
    {
      printf("Please enter a move (you have %zu wall%sleft):\n$ ", game.nb_walls[self.id], (game.nb_walls[self.id] == 1) ? " "  : "s ");
      get_input(input, MAX_INPUT_LENGTH);
      move = move_from_input(input);
    }
  if (move.t == MOVE)
    game.pl_pos[self.id] = move.m;
  else
    {
      game.nb_walls[self.id]--;
      place_wall(game.graph, move.e);
    }
  return move;
}


void finalize()
{
  client_free(&game, &self);
}

