#include "test_graph_utils.h"
#include "test_queue_uint.h"
#include "test_rules.h"
#include "test_shortest_path.h"
#include "test_clients_commons.h"
#include "test_strat_rush.h"
//#include "test_greedy.h"

int main()
{
  test_graph_utils();
  test_queue_uint();
  test_rules();
  test_shortest_path();
  test_clients_commons();
  test_strat_rush();
  //  test_greedy();
  return 0;
}
