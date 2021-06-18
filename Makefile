GSL_PATH ?= /us
CFLAGS = -Wall -Wextra -std=c99 -g --coverage
GCCOBJ = gcc ${CFLAGS} -I${GSL_PATH}/include -c
GCCEXE = gcc ${CFLAGS} -I${GSL_PATH}/include -L${GSL_PATH}/lib
GSLLIBS = -lgsl -lgslcblas -lm


####################################################################
##                         General rules
####################################################################

build: graph_utils.o game_graphs.o queue_uint.o rules.o shortest_path.o game_graphs.o graph_debug.o input.o strat_rush.o strat_greedy.o strat_shatranj.o clients_commons.o cl_rush.o cl_greedy.o cl_shatranj.o cl_interactive.o cl_sidewall_opening.o

test: test_graph_utils.o test_queue_uint.o test_rules.o test_shortest_path.o test_clients_commons.o test_strat_rush.o

install: test alltests server rush greedy shatranj sidewall_opening

clean:
	find -type f -name '*.o' -delete
	find -type f -name '*.gcno' -delete
	find -type f -name '*.gcda' -delete
	find -type f -name '*.gcov' -delete
	rm -rf install/* 


####################################################################
##                         Object files
####################################################################

graph_utils.o:
	${GCCOBJ} src/graph_utils.c -o src/graph_utils.o

game_graphs.o:
	${GCCOBJ} src/game_graphs.c -o src/game_graphs.o

queue_uint.o:
	gcc ${CFLAGS} -fpic -c src/queues/queue_uint.c -o src/queues/queue_uint.o

rules.o:
	${GCCOBJ} src/rules.c -o src/rules.o

shortest_path.o:
	${GCCOBJ} src/clients/shortest_path.c -o src/clients/shortest_path.o

graph_debug.o:
	gcc -Wall -Wextra -std=c99 -I${GSL_PATH}/include -c src/graph_debug.c -o src/graph_debug.o	

input.o:
	gcc -Wall -Wextra -std=c99 -I${GSL_PATH}/include -c src/input.c -o src/input.o

player.o:
	${GCCOBJ} src/player.c -o src/player.o


## Strategies
strat_rush.o:
	${GCCOBJ} src/clients/strat_rush.c -o src/clients/strat_rush.o

strat_greedy.o:
	${GCCOBJ} src/clients/strat_greedy.c -o src/clients/strat_greedy.o

strat_shatranj.o:
	${GCCOBJ} src/clients/strat_shatranj.c -o src/clients/strat_shatranj.o



## Clients
clients_commons.o:
	${GCCOBJ} src/clients/clients_commons.c -o src/clients/clients_commons.o

cl_rush.o:
	${GCCOBJ} -fpic src/clients/cl_rush.c -o src/clients/cl_rush.o

cl_greedy.o:
	${GCCOBJ} -fpic src/clients/cl_greedy.c -o src/clients/cl_greedy.o

cl_shatranj.o:
	${GCCOBJ} -fpic src/clients/cl_shatranj.c -o src/clients/cl_shatranj.o

cl_interactive.o:
	${GCCOBJ} -fpic src/clients/cl_interactive.c -o src/clients/cl_interactive.o

cl_sidewall_opening.o:
	${GCCOBJ} -fpic src/clients/cl_sidewall_opening.c -o src/clients/cl_sidewall_opening.o

####################################################################
##                         Executables
####################################################################

server: 
	${GCCEXE} src/graph_utils.o src/game_graphs.o src/queues/queue_uint.o src/rules.o src/input.o src/graph_debug.o src/main.c ${GSLLIBS} -ldl -o install/server

alltests:
	${GCCEXE} src/graph_debug.o src/graph_utils.o test/test_graph_utils.o src/queues/queue_uint.o test/test_queue_uint.o src/rules.o test/test_rules.o src/clients/shortest_path.o test/test_shortest_path.o src/clients/strat_rush.o test/test_strat_rush.o src/clients/clients_commons.o test/test_clients_commons.o test/test_all.c ${GSLLIBS} -o install/alltests

rush:
	gcc ${CFLAGS} -shared src/graph_utils.o src/queues/queue_uint.o src/rules.o src/clients/clients_commons.o src/clients/shortest_path.o src/clients/strat_rush.o src/clients/cl_rush.o -o install/rush.so

greedy:
	gcc ${CFLAGS} -shared src/graph_utils.o src/queues/queue_uint.o src/rules.o src/clients/clients_commons.o src/clients/shortest_path.o src/clients/strat_rush.o src/clients/strat_greedy.o src/clients/cl_greedy.o -o install/greedy.so

shatranj:
	gcc ${CFLAGS} -shared src/graph_utils.o src/queues/queue_uint.o src/rules.o src/clients/clients_commons.o src/clients/shortest_path.o src/clients/strat_rush.o src/clients/cl_shatranj.o -o install/shatranj.so

interactive:
	gcc ${CFLAGS} -shared src/graph_utils.o src/queues/queue_uint.o src/rules.o src/clients/clients_commons.o src/clients/cl_interactive.o -o install/interactive.so

sidewall_opening:
	gcc ${CFLAGS} -shared src/graph_utils.o src/queues/queue_uint.o src/rules.o src/clients/clients_commons.o src/clients/shortest_path.o src/clients/strat_rush.o src/clients/strat_greedy.o src/clients/cl_sidewall_opening.o -o install/sidewall_opening.so


####################################################################
##                         Tests
####################################################################

test_graph_utils.o:
	${GCCOBJ} test/test_graph_utils.c -o test/test_graph_utils.o

test_queue_uint.o:
	${GCCOBJ} test/test_queue_uint.c -o test/test_queue_uint.o

test_rules.o:
	${GCCOBJ} test/test_rules.c -o test/test_rules.o

test_shortest_path.o:
	${GCCOBJ} test/test_shortest_path.c -o test/test_shortest_path.o

test_strat_rush.o:
	${GCCOBJ} test/test_strat_rush.c -o test/test_strat_rush.o

test_greedy.o:
	${GCCOBJ} test/test_greedy.c -o test/test_greedy.o

test_clients_commons.o:
	${GCCOBJ} test/test_clients_commons.c -o test/test_clients_commons.o

test_game_graph:
	${GCCEXE} src/game_graphs.o test/test_game_graph.c ${GSLLIBS} -o test/test_graph.o

test_game_play:
	${GCCEXE} src/game_graphs.o src/player.o test/test_game_play.c ${GSLLIBS} -o test/test_play
