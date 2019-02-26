#include "bus_manager.h"
#include "query.h"

using namespace std;

int main() {
#ifdef RUN_UNIT_TESTS
  TestOperatorVector();
  TestOperatorQuery();
  {  
    BusManager bm;
    bm.TestAddBus();
  }
  {
    BusManager bm;
    bm.TestGetBusesForStop();
  }
  {
    BusManager bm;
    bm.TestGetStopsForBus();
  }
  {
    BusManager bm;
    bm.TestGetAllBuses();
  }
  return 0;
#else
/**********************************************/
  int query_count;
  Query q;

  cin >> query_count;

  BusManager bm;
  for (int i = 0; i < query_count; ++i) {
    cin >> q;
    switch (q.type) {
    case QueryType::NewBus:
      bm.AddBus(q.bus, q.stops);
      break;
    case QueryType::BusesForStop:
      cout << bm.GetBusesForStop(q.stop) << endl;
      break;
    case QueryType::StopsForBus:
      cout << bm.GetStopsForBus(q.bus) << endl;
      break;
    case QueryType::AllBuses:
      cout << bm.GetAllBuses() << endl;
      break;
    }
  }

  return 0;
#endif
}
