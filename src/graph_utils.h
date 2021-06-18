#ifndef _QUOR_GRAPH_UTILS_H_
#define _QUOR_GRAPH_UTILS_H_

#include "graph.h"


/* Various functions that make it easier to manipulate graph_t structures. */


/* A pointer to a heap-allocated square graph of width m */
struct graph_t* graph__initialize(size_t m);


/* A pointer to a heap-allocated copy of a graph */
struct graph_t* graph__copy(struct graph_t* g);

/* Free the memory occupied by a graph */
void graph__free(struct graph_t* g);

/* A more concise gsl_spmatrix_uint_get */
unsigned int matrix_get(gsl_spmatrix_uint* m, size_t v1, size_t v2);


/* The northern/southern/western/eastern neighbor of a vertex
 * SIZE_MAX if the vertex has no neighbor in direction dir */
size_t vertex_neighbor(struct graph_t* g, size_t v1, size_t dir);

/* A predicate telling if there is an edge between two vertices  */
int edge_exists(struct graph_t* g, size_t v1, size_t v2);

/* Delete the edge between between two vertices or return non-zero in case
 * said edge doesn't exist
 * POSTCOND:
 * - g->t[v1][v2] = g->t[v2][v1] = 0 */
int disconnect_vertices(struct graph_t* g, size_t v1, size_t v2);

/* Remove two edges in a graph */
void place_wall(struct graph_t* g, struct edge_t wall[2]);

/* Create two edges in a graph */
void remove_wall(struct graph_t* g, struct edge_t wall[2]);

/* A predicate telling if a vertex is isolated */
int vertex_is_isolated(struct graph_t* g, size_t v);
  
/* Delete all the edges connected to a vertex or return non-zero in case
 * said vertex is already isolated */
int isolate_vertex(struct graph_t* g, size_t v);



#endif // _QUOR_GRAPH_UTILS_H_


