#include <iostream>
#include "test_runner.h"
#include "misc.h"
#include "io.h"
#include "json.h"
#include <string>
#include <iomanip>
#include "graph.h"
#include "router.h"

using namespace std;

int main(void) {
#if 1

  stringstream ss{R"(
{
  "routing_settings": {
    "bus_wait_time": 2,
    "bus_velocity": 30
  },
  "base_requests": [
    {
      "type": "Bus",
      "name": "289",
      "stops": [
        "Zagorye",
        "Lipetskaya ulitsa 46",
        "Lipetskaya ulitsa 40",
        "Lipetskaya ulitsa 40",
        "Lipetskaya ulitsa 46",
        "Moskvorechye",
        "Zagorye"
      ],
      "is_roundtrip": true
    },
    {
      "type": "Stop",
      "road_distances": {
        "Lipetskaya ulitsa 46": 230
      },
      "longitude": 37.68372,
      "name": "Zagorye",
      "latitude": 55.579909
    },
    {
      "type": "Stop",
      "road_distances": {
        "Lipetskaya ulitsa 40": 390,
        "Moskvorechye": 12400
      },
      "longitude": 37.682205,
      "name": "Lipetskaya ulitsa 46",
      "latitude": 55.581441
    },
    {
      "type": "Stop",
      "road_distances": {
        "Lipetskaya ulitsa 40": 1090,
        "Lipetskaya ulitsa 46": 380
      },
      "longitude": 37.679133,
      "name": "Lipetskaya ulitsa 40",
      "latitude": 55.584496
    },
    {
      "type": "Stop",
      "road_distances": {
        "Zagorye": 10000
      },
      "longitude": 37.638433,
      "name": "Moskvorechye",
      "latitude": 55.638433
    }
  ],
  "stat_requests": [
    {
      "type": "Bus",
      "name": "289",
      "id": 1
    },
    {
      "type": "Route",
      "from": "Zagorye",
      "to": "Moskvorechye",
      "id": 2
    },
    {
      "type": "Route",
      "from": "Moskvorechye",
      "to": "Zagorye",
      "id": 3
    },
    {
      "type": "Route",
      "from": "Lipetskaya ulitsa 40",
      "to": "Lipetskaya ulitsa 40",
      "id": 4
    }
  ]
}
  )"};
    BusDatabaseHandler handler;

    Json::Document doc = Json::Load(cin);
    auto responses = handler.ReadRequests(doc).ProcessRequests().GetResponses();
    cout << setprecision(6);
    Json::Print(responses, cout);
#else
    Graph::DirectedWeightedGraph<int> graph(0);
    cout << graph.GetEdgeCount() << endl;
    cout << graph.GetVertexCount() << endl;
    graph = Graph::DirectedWeightedGraph<int>(10);
    cout << graph.GetEdgeCount() << endl;
    cout << graph.GetVertexCount() << endl;
    graph.AddEdge({0,5,6});
    graph.AddEdge({5,6,5});
    graph.AddEdge({5,6,6});
    cout << graph.GetEdgeCount() << endl;
    Graph::Router router(graph);
    auto info = router.BuildRoute(0,6);
    cout << endl;
    cout << router.GetRouteEdge(info->id,0) << endl;
    cout << router.GetRouteEdge(info->id,1) << endl;
    cout << router.GetRouteEdge(info->id,2) << endl;
    cout << router.GetRouteEdge(info->id,3) << endl;
    cout << router.GetRouteEdge(info->id,4) << endl;
#endif
    
    return 0;
}