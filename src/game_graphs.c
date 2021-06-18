#include "game_graphs.h"
#include <stdio.h>



void annuling__line(int k, size_t m, gsl_spmatrix_uint* t){

  int n = m*m;
  
  for (int j=0 ; j < n ; ++j){
    gsl_spmatrix_uint_set(t,k,j,0);
  }
  
}

void annuling__column(int k, size_t m, gsl_spmatrix_uint* t){

  int n = m*m;
  
  for (int i=0 ; i < n ; ++i){
    gsl_spmatrix_uint_set(t,i,k,0);
  }
  
}

void make__holes(int s, int height, int width, gsl_spmatrix_uint* t, size_t m){

  for (int i=0 ; i < height ; ++i){
    for (int j=0 ; j < width ; ++j){
      annuling__line(s+j+m*i,m,t);
      annuling__column(s+j+m*i,m,t);
    }
  }
  
}


// set m[l][s::e] to 0
void reset_line(gsl_spmatrix_uint* m, size_t l, size_t s, size_t e)
{
  for (size_t i = s; i < e; i++)
    gsl_spmatrix_uint_set(m, l, i, 0);
}	

  
// Create the full graph
struct graph_t* full__graph(size_t m){
  return graph__initialize(m);
}

// Create the toric graph
struct graph_t* toric__graph(size_t m){

  struct graph_t* g = graph__initialize(m);

  int height = m/3;
  
  int top_left_rectangle_vertex = (m+1)*height;

  make__holes(top_left_rectangle_vertex,height,height,g->t,m);
  
  return g;
  
}

struct graph_t* H__graph(size_t m){
  struct graph_t* g = graph__initialize(m);

  int height = m/3;

  int top_left_first_rectangle_vertex = height;
  
  int top_left_second_rectangle_vertex = height + 2*height*m;
  
  make__holes(top_left_first_rectangle_vertex,height,height,g->t,m);

  make__holes(top_left_second_rectangle_vertex,height,height,g->t,m);

  reset_line(g->o, BLACK, height, height+height);
  reset_line(g->o, WHITE, g->num_vertices-m+height, g->num_vertices-height);
  
  return g;
}

struct graph_t* snake__graph(size_t m){
  struct graph_t* g = graph__initialize(m);

  int height = m/5;
  int width = 4*height;
  
  int top_left_first_rectangle_vertex = height;
  int top_left_second_rectangle_vertex = 2*m*height;
  int top_left_third_rectangle_vertex = (4*m+1)*height;

  make__holes(top_left_first_rectangle_vertex,height,width,g->t,m);
  make__holes(top_left_second_rectangle_vertex,height,width,g->t,m);
  make__holes(top_left_third_rectangle_vertex,height,width,g->t,m);
  reset_line(g->o, BLACK, height, m-1);
  reset_line(g->o, WHITE, g->num_vertices-m+height, g->num_vertices-1);
  return g;
}


struct graph_t* generating__graph(char option,size_t m){

  switch (option)
    {
    case 'c':
      return full__graph(m);
    case 't':
      return toric__graph(m);
    case 'h':
      return H__graph(m);
    case 's':
      return snake__graph(m);
    default:
      printf("Error, wrong option choosed.");
      exit(EXIT_FAILURE);
    };
}















  
  
