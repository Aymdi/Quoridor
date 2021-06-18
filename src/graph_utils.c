#include "graph_utils.h"


// a pointer to a square graph
struct graph_t* graph__initialize(size_t m)
{
  struct graph_t* g = malloc(sizeof(struct graph_t));
  
  g->num_vertices = m * m;
  size_t n = g->num_vertices;

  gsl_spmatrix_uint* t = gsl_spmatrix_uint_alloc(n, n);
  for (size_t v = 0; v < n; v++)
    {
      if (v >= m) // northern neighbor exists
	gsl_spmatrix_uint_set(t, v, v-m, 1);
      
      if (v < n-m) // southern neighbor exists	
	gsl_spmatrix_uint_set(t, v, v+m, 2);
      
      if (v%m != 0) // western neighbor exists
	gsl_spmatrix_uint_set(t, v, v-1, 3);

      if (v%m != m-1) // eastern neighbor exists
	gsl_spmatrix_uint_set(t, v, v+1, 4);
    }
  g->t = t;

  gsl_spmatrix_uint* o = gsl_spmatrix_uint_alloc(2, n);
  for (size_t v = 0; v < m; v++)
    {
      gsl_spmatrix_uint_set(o, BLACK, v, 1);
      gsl_spmatrix_uint_set(o, WHITE, n-1-v, 1);
    }
  g->o = o;

  return g;
}


// a pointer to a heap-allocated copy of a graph
struct graph_t* graph__copy(struct graph_t* g)
{
  struct graph_t* copy = malloc(sizeof(struct graph_t));
  size_t n = g->num_vertices;
  
  copy->num_vertices = n;
  copy->t = gsl_spmatrix_uint_alloc(n, n);
  copy->o = gsl_spmatrix_uint_alloc(2, n);
  gsl_spmatrix_uint_memcpy(copy->t, g->t);
  gsl_spmatrix_uint_memcpy(copy->o, g->o);
  return copy;
}
  
  
// free the memory occupied by a graph
void graph__free(struct graph_t* g)
{
  gsl_spmatrix_uint_free(g->t);
  gsl_spmatrix_uint_free(g->o);
  free(g);
}


// a more concise gsl_spmatrix_uint_get
unsigned int matrix_get(gsl_spmatrix_uint* m, size_t v1, size_t v2)
{
  return gsl_spmatrix_uint_get(m, v1, v2);
}


// the neighbor of a vertex
// SIZE_MAX if the vertex has no neighbor in direction dir
size_t vertex_neighbor(struct graph_t* g, size_t v, size_t dir)
{
  size_t m = (size_t) sqrt(g->num_vertices);
  size_t neighbor;
  switch (dir)
    {
    case 1: neighbor = v-m; break;
    case 2: neighbor = v+m; break;
    case 3: neighbor = v-1; break;
    case 4: neighbor = v+1; break;
    default: neighbor = SIZE_MAX;
    }
  if (neighbor < g->num_vertices && matrix_get(g->t, v, neighbor) == dir)
    return neighbor;
  return SIZE_MAX;
}


// a predicate telling if an edge between two vertices exists
int edge_exists(struct graph_t* g, size_t v1, size_t v2)
{
  if (v1 < g->num_vertices && v2 < g->num_vertices)
    return (matrix_get(g->t, v1, v2) && matrix_get(g->t, v2, v1));
  return 0;
}


// delete the edge between between two vertices
// returns non-zero in case said edge doesn't exist
int disconnect_vertices(struct graph_t* g, size_t v1, size_t v2)
{
  if (!matrix_get(g->t, v1, v2) || !matrix_get(g->t, v2, v1))
    return 1;
  gsl_spmatrix_uint_set(g->t, v1, v2, 0);
  gsl_spmatrix_uint_set(g->t, v2, v1, 0);
  return 0;
}


// remove two edges
void place_wall(struct graph_t* g, struct edge_t w[2])
{
  for (size_t edge = 0; edge < 2; edge ++)
    disconnect_vertices(g, w[edge].fr, w[edge].to);
}


// create two edges
void remove_wall(struct graph_t* g, struct edge_t w[2])
{
  for (size_t edge = 0; edge < 2; edge ++)
    {
      size_t v1 = (w[edge].fr > w[edge].to) ? w[edge].fr : w[edge].to;
      size_t v2 = (w[edge].fr > w[edge].to) ? w[edge].to : w[edge].fr;
      size_t diff = v1-v2;
      if (diff == 1) // v1 is to the right of v2
	{
	  gsl_spmatrix_uint_set(g->t, v1, v2, 3);
	  gsl_spmatrix_uint_set(g->t, v2, v1, 4);
	}
      else // v1 is below v2
	{
	  gsl_spmatrix_uint_set(g->t, v1, v2, 1);
	  gsl_spmatrix_uint_set(g->t, v2, v1, 2);
	}
    }
}


/* A predicate telling if a vertex is isolated */
int vertex_is_isolated(struct graph_t* g, size_t v)
{
  int isolated = 1;
  for (size_t i = 0; i < g->num_vertices; i++)
    {
      if (gsl_spmatrix_uint_get(g->t, v, i))
	isolated = 0;
    }
  return isolated;
}


// delete every edge connected to a vertex
// returns non-zero in case said vertex is already isolated
int isolate_vertex(struct graph_t* g, size_t v)
{
  size_t m = (size_t) sqrt(g->num_vertices);
  size_t neighbors[4] = {v-m, v+m, v-1, v+1};
  for (size_t i = 0; i < 4; i++)
    {
      if (neighbors[i] < g->num_vertices)
	disconnect_vertices(g, v, neighbors[i]);
    }
  return 0;
}

