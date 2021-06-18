#include "../src/game_graphs.h"
#include "../src/struct_player.h"
#include "../src/player.h"
#include "../src/play.h"
#include "../src/graph.h"


#include <stdio.h>
#include <stdlib.h>

#define TESTCASE(msg, f) printf("%s...%s\n", msg, f() ? "\033[32mPASSED\033[m" : "\033[31mFAILED\033[m")

struct player p;

int TESTPASSED(){
    return 1;
}


int test__get_player_name(){
    struct graph_t* g = graph__empty();
    g = graph__initialize(20);
    initialize(0,g,10);
    char const *name = get_player_name();
    return(name == "player1" );
}

int test__can_move(){
    p.pos[0]=0;
    p.pos[1]=399;
    int r1 = can__move(0,1) && can__move(0,20) && !can__move(0,80);
    p.pos[0]=30;
    int r2 = can__move(30,31) && can__move(30,29) && can__move(30,10) && can__move(30,50);
    p.pos[1]=31;
    int r3 = can__move(30,32) && !can__move(30,31);
    
}


int main(){
    TESTCASE("TESTPASSED", TESTPASSED);
    TESTCASE("test__get_player_name", test__get_player_name);
    TESTCASE("test__can_move", test__can_move);
    return 0;
}