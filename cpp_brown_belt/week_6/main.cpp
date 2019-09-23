#include <iostream>
#include "test_runner.h"
#include "misc.h"
#include "io.h"
#include "json.h"
#include <string>
#include <iomanip>

using namespace std;

void JsonPrint(const Json::Node& top, ostream& os) {
  if(holds_alternative<int>(top)) {
    os << top.AsInt();
  } else if (holds_alternative<double>(top)) {
    os << top.AsDouble();
  } else if (holds_alternative<string>(top)) {
    os << "\"" << top.AsString() << "\"";
  } else if (holds_alternative<bool>(top)) {
    os << top.AsBool();
  } else if (holds_alternative<std::vector<Json::Node>>(top)) {
    os << "[";
    const auto& as_array = top.AsArray();
    if(as_array.size()) { 
      for(const auto& node : Range(as_array.begin(),prev(as_array.end()))) {
        JsonPrint(node,os);
        os << ',';
      }
     
      JsonPrint(*prev(as_array.end()),os);
    }
    // os << '\b';
    os << "]";
  } else {
    const auto& as_map = top.AsMap();
    os << "{";
    if(as_map.size()) {
      for(const auto& pair_ : Range(as_map.begin(),prev(as_map.end()))) {
        os << "\"" << pair_.first << "\": ";
        JsonPrint(pair_.second,os);
        os << ',';
      } 
      os << "\"" << prev(as_map.end())->first << "\": ";
      JsonPrint(prev(as_map.end())->second,os);
    }
    // os << '\b';
    os << "}";
  }
}

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
    }
  ]
}
  )"};
    BusDatabaseHandler handler;

    Json::Document doc = Json::Load(ss);
    auto responses = handler.ReadRequests(doc).ProcessRequests().GetResponses();
    cout << setprecision(6);
    JsonPrint(responses, cout);
#else
    try {
      Json::Node node{0.5};
      double lol = node.AsDouble();
      cout << lol << endl;
    } catch(exception& e) {
        cerr << e.what() << endl;
    }
#endif
    
    return 0;
}