#include "test_graph_utils.h"


static void print_green_ok()
{
  printf("\x1b[32;1mOK\x1b[0m\n");
}


void test_graph__initialize()
{
  printf("> graph__initialize() - ");
  size_t m = 3;
  struct graph_t* g = graph__initialize(m);  
  assert(g->num_vertices == m*m);
  for (size_t v = 0; v < g->num_vertices; v++)
    {
      if (v > 2)
	assert(gsl_spmatrix_uint_get(g->t, v, v-m) == 1);
      if (v < 6)
	assert(gsl_spmatrix_uint_get(g->t, v, v+m) == 2);
      if (v%3)
	assert(gsl_spmatrix_uint_get(g->t, v, v-1) == 3);
      if (v%3 < 2)
	assert(gsl_spmatrix_uint_get(g->t, v, v+1) == 4);
    }
  for (size_t v = 0; v < g->num_vertices; v++)
    {
      if (v < 3)
	assert(gsl_spmatrix_uint_get(g->o, BLACK, v) == 1);
      else
	assert(gsl_spmatrix_uint_get(g->o, BLACK, v) == 0);
      if (v > 5)
	assert(gsl_spmatrix_uint_get(g->o, WHITE, v) == 1);
      else
	assert(gsl_spmatrix_uint_get(g->o, WHITE, v) == 0);
    }
  graph__free(g);
  print_green_ok();
}


void test_graph__copy()
{
  printf("> graph__copy() - ");
  size_t m = 3;
  struct graph_t* g1 = graph__initialize(m);
  struct graph_t* g2 = graph__copy(g1);
  assert(g2->num_vertices == g1->num_vertices);
  assert(gsl_spmatrix_uint_equal(g1->t, g2->t));
  assert(gsl_spmatrix_uint_equal(g1->o, g2->o));

  graph__free(g1);
  graph__free(g2);
  print_green_ok();
}


void test_matrix_get()
{
  printf("> matrix_get() - ");
  size_t n = 3;
  gsl_spmatrix_uint* m = gsl_spmatrix_uint_alloc(n, n);
  assert(matrix_get(m, 0, 0) == 0);
  
  gsl_spmatrix_uint_set(m, 0, 0, 8);
  assert(matrix_get(m, 0, 0) == 8);
  
  gsl_spmatrix_uint_free(m);
  print_green_ok();
}


void test_vertex_neighbor()
{
  printf("> vertex_neighbor() - ");
  size_t m = 3;
  struct graph_t* g = graph__initialize(m);
  assert(vertex_neighbor(g, 0, 1) == SIZE_MAX);
  assert(vertex_neighbor(g, 0, 2) == 3);
  assert(vertex_neighbor(g, 0, 3) == SIZE_MAX);
  assert(vertex_neighbor(g, 0, 4) == 1);
  assert(vertex_neighbor(g, 8, 4) == SIZE_MAX);
  
  graph__free(g);
  print_green_ok();
}


void test_edge_exists()
{
  printf("> edge_exists() - ");
  size_t m = 3;
  struct graph_t* g = graph__initialize(m);
  assert(edge_exists(g, 0, 1));
  assert(edge_exists(g, 0, 3));
  assert(edge_exists(g, 0, 4) == 0);
  
  gsl_spmatrix_uint_set(g->t, 0, 1, 0);
  assert(edge_exists(g, 0, 1) == 0);
  
  graph__free(g);
  print_green_ok();
}


void test_disconnect_vertices()
{
  printf("> disconnect_vertices() - ");
  size_t m = 3;
  struct graph_t* g = graph__initialize(m);
  assert(edge_exists(g, 0, 1));
  disconnect_vertices(g, 0, 1);
  assert(matrix_get(g->t, 0, 1) == 0);
  assert(matrix_get(g->t, 1, 0) == 0);

  graph__free(g);
  print_green_ok();
}


void test_place_wall()
{
  printf("> place_wall() - ");
  size_t m = 3;
  struct graph_t* g = graph__initialize(m);
  struct edge_t wall[2] = {{0, 1}, {3, 4}};
  assert(edge_exists(g, 0, 1));
  assert(edge_exists(g, 3, 4));
  place_wall(g, wall);
  assert(edge_exists(g, 0, 1) == 0);
  assert(edge_exists(g, 3, 4) == 0);

  graph__free(g);
  print_green_ok();
}


void test_remove_wall()
{
  printf("> remove_wall() - ");
  size_t m = 3;
  struct graph_t* g = graph__initialize(m);
  struct edge_t wall[2] = {{0, 1}, {3, 4}};
  place_wall(g, wall);
  assert(edge_exists(g, 0, 1) == 0);
  assert(edge_exists(g, 3, 4) == 0);

  remove_wall(g, wall);
  assert(edge_exists(g, 0, 1));
  assert(edge_exists(g, 3, 4));

  graph__free(g);
  print_green_ok();
}

  
void test_isolate_vertex()
{
  printf("> isolate_vertex() - ");
  size_t m = 3;
  struct graph_t* g = graph__initialize(m);
  isolate_vertex(g, 4);
  assert(edge_exists(g, 4, 1) == 0);
  assert(edge_exists(g, 4, 3) == 0);
  assert(edge_exists(g, 4, 5) == 0);
  assert(edge_exists(g, 4, 7) == 0);

  isolate_vertex(g, 0);
  assert(edge_exists(g, 0, 1) == 0);
  assert(edge_exists(g, 0, 3) == 0);

  graph__free(g);
  print_green_ok();
}

  
void test_graph_utils()
{
  printf("TESTING: graph_utils.c\n");
  test_graph__initialize();
  test_graph__copy();
  test_matrix_get();
  test_vertex_neighbor();
  test_edge_exists();
  test_disconnect_vertices();
  test_place_wall();
  test_remove_wall();
  test_isolate_vertex();
  printf("All tests OK\n\n");
}
  
  
