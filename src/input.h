#include <getopt.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// parse optional arguments -m and -t
void parse_opts(int argc, char* argv[], size_t *width, char *shape, int* interface);

// check if optional arguments have the correct format
void check_opts(size_t width, char shape);

// check if the last 2 parameters are .so files
void check_clients(int argc, char* argv[]);

// display message following by an EXIT_FAILURE
void exit_message(char* message);
