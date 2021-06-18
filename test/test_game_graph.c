#include "../src/game_graphs.h"
#include <assert.h>
#include <stdio.h>

void test_graph__initialize(){
  printf("> graph__initialize -");

  struct graph_t* g = graph__empty();
  g = graph__initialize(20);

  

}


int main(){


  return 0;
}



