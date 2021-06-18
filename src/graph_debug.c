#include "graph_debug.h"


// display the adjacency matrix of a graph in text format
void graph__matrix_display(struct graph_t* g)
{
  for (size_t i = 0; i < g->t->size1; i++)
    {
      for (size_t j = 0; j < g->t->size2; j++)
	{
	  if (gsl_spmatrix_uint_get(g->t, i, j))
	    printf("%d ", gsl_spmatrix_uint_get(g->t, i, j));
	  else
	    printf(". ");
	}
      printf("\n");
    }
}

void graph__matrix_o_display(struct graph_t* g)
{
  for (size_t p = 0; p <2; p++)
  {
    printf("Player %ld : ", p);
    for (size_t i = 0; i<g->num_vertices; i++)
    {
      printf("%d ", gsl_spmatrix_uint_get(g->o, p, i));
    }
    printf("\n");
  }
}


void display_line(struct graph_t* g, size_t l, size_t m, size_t pl_pos[])
{
  for (size_t v = l*m; v < l*m+m; v++)
    {
      if (v >= m && gsl_spmatrix_uint_get(g->t, v, v-m))
	printf("    ");
      else
	printf(" ---");
    }
  printf("\n|");
  for (size_t v = l*m; v < l*m+m; v++)
    {
      if (v == pl_pos[0])
	printf(" 0 ");
      else
	{
	  if (v == pl_pos[1])
	    printf(" 1 ");
	  else
	    {
	      printf("\e[0;30m");
	      if (v < 100)
		printf(" ");
	      printf("%zu\e[0m", v);
	      if (v < 10)
		printf(" ");
	    }
	}
      if (v%m < m-1 && gsl_spmatrix_uint_get(g->t, v, v+1))
	printf(" ");
      else
	printf("|");
    }
  printf("\n");
}	  
      

void display_graph(struct graph_t* g, size_t pl_pos[])
{
  size_t m = (size_t) sqrt(g->num_vertices);
  for (size_t l = 0; l < m; l++)
    display_line(g, l, m, pl_pos);
  for (size_t v = m*m-m; v < m*m; v++)
    printf(" ---");
  printf("\n");
}
      
