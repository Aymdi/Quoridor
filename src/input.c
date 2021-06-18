#include "input.h"


void exit_message(char* message)
{
  printf("%s\n", message);
  exit(EXIT_FAILURE);
}


// check if optional arguments have the correct format
void check_opts(size_t width, char shape)
{
  if (width <= 2)
    exit_message("ERROR:\n> Invalid board width (-m), must be > 2.");
  
  if (!(shape == 'c' || shape == 't' || shape == 'h' || shape == 's'))
    exit_message("ERROR:\n> Invalid board shape (-t). Available values are :\n  c: square shape\n  t: toric shape\n  h: H shape\n  s: snake shape");
  
  if (shape == 't' && width%3)
    exit_message("ERROR:\n> Incompatible shape (-t) and width (-m). Toric graph requires a width that is a multiple of 3.");
  
  if (shape == 'h' && width%3)
    exit_message("ERROR:\n> Incompatible shape (-t) and width (-m). H graph requires a width that is a multiple of 3.");
  
  if (shape == 's' && width%5)
    exit_message("ERROR:\n> Incompatible shape (-t) and width (-m). Snake graph requires a width that is a multiple of 5.");
}


// parse optional arguments -m and -t
void parse_opts(int argc, char* argv[], size_t *width, char *shape, int* interface)
{
  int opt;
  while ((opt = getopt(argc, argv, "m:t:i")) != -1)
    {
      switch (opt)
	      {
	        case 'm': *width = atoi(optarg); break;
	        case 't': *shape = *optarg; break;
	        case 'i': *interface = 1; break;
	        default: fprintf(stderr, "Usage : %s [-m width] [-t shape] \n", argv[0]);
	        exit(EXIT_FAILURE);
	      }
    }
  check_opts(*width, *shape);
}


// check if the last 2 parameters are .so files
void check_clients(int argc, char* argv[])
{
  if (argc >= 3)
    {
      char* client1 = strstr(argv[argc-2], ".so");
      char* client2 = strstr(argv[argc-1], ".so");
      if (client1 && client2)
	{
	  if (strlen(client1) == 3 && strlen(client2) == 3)
	    return;
	}
    }
  printf("ERROR:\n> Invalid client names. Correct format:\n");
  printf("  ./server [-m width] [-t shape] player1.so player2.so\n");
  exit(EXIT_FAILURE);
}
