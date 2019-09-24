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
    "bus_wait_time": 6,
    "bus_velocity": 40
  },
  "base_requests": [
    {
      "type": "Bus",
      "name": "297",
      "stops": [
        "Biryulyovo Zapadnoye",
        "Biryulyovo Tovarnaya",
        "Universam",
        "Biryulyovo Zapadnoye"
      ],
      "is_roundtrip": true
    },
    {
      "type": "Bus",
      "name": "635",
      "stops": [
        "Biryulyovo Tovarnaya",
        "Universam",
        "Prazhskaya"
      ],
      "is_roundtrip": false
    },
    {
      "type": "Stop",
      "road_distances": {
        "Biryulyovo Tovarnaya": 2600
      },
      "longitude": 37.6517,
      "name": "Biryulyovo Zapadnoye",
      "latitude": 55.574371
    },
    {
      "type": "Stop",
      "road_distances": {
        "Prazhskaya": 4650,
        "Biryulyovo Tovarnaya": 1380,
        "Biryulyovo Zapadnoye": 2500
      },
      "longitude": 37.645687,
      "name": "Universam",
      "latitude": 55.587655
    },
    {
      "type": "Stop",
      "road_distances": {
        "Universam": 890
      },
      "longitude": 37.653656,
      "name": "Biryulyovo Tovarnaya",
      "latitude": 55.592028
    },
    {
      "type": "Stop",
      "road_distances": {},
      "longitude": 37.603938,
      "name": "Prazhskaya",
      "latitude": 55.611717
    }
  ],
  "stat_requests": [
    {
      "type": "Bus",
      "name": "297",
      "id": 1
    },
    {
      "type": "Bus",
      "name": "635",
      "id": 2
    },
    {
      "type": "Stop",
      "name": "Universam",
      "id": 3
    },
    {
      "type": "Route",
      "from": "Biryulyovo Zapadnoye",
      "to": "Universam",
      "id": 4
    },
    {
      "type": "Route",
      "from": "Biryulyovo Zapadnoye",
      "to": "Prazhskaya",
      "id": 5
    }
  ]
}
  )"};
    BusDatabaseHandler handler;

    Json::Document doc = Json::Load(ss);
    cerr << "HERE" << endl;
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