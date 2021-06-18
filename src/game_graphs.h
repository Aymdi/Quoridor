#include "graph_utils.h"


// Choosing the graph to generate
struct graph_t* generating__graph(char option,size_t m);

// Create the full graph
struct graph_t* full__graph(size_t m);

// Create the toric graph
struct graph_t* toric__graph(size_t m);

// Create the H-graph
struct graph_t* H__graph(size_t m);

// Create the serpent graph
struct graph_t* snake__graph(size_t m);

// Function that annul an entire line of the adjacency matrix
void annuling__line(int k, size_t m, gsl_spmatrix_uint* t);

// Function that annul an entire column of the adjacency matrix
void annuling__column(int k, size_t m, gsl_spmatrix_uint* t);

// Function that diggs holes in the graph, more precisely in the adjacency matrix
void make__holes(int s, int height, int width, gsl_spmatrix_uint* t, size_t m);




