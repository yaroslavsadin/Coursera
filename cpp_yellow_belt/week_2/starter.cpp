#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

template<typename T> ostream& operator<<(ostream& os, const vector<T>& v) {
  constexpr char delim = '\n';
  bool first = true;
  for(const auto& i : v) {
    if(!first) {
      os << delim;
    }
    first = false;
    os << i;
  }
  return os;
}

enum class QueryType {
  NewBus,
  BusesForStop,
  StopsForBus,
  AllBuses
};

struct Query {
  QueryType type;
  string bus;
  string stop;
  vector<string> stops;
};

bool operator==(const Query& lhs, const Query rhs) {
  return (tie(lhs.bus,lhs.stop,lhs.stops,lhs.type) == 
          tie(rhs.bus,rhs.stop,rhs.stops,rhs.type));
}

istream& operator >> (istream& is, Query& q) {
  // Реализуйте эту функцию
  string operation_code;
  is >> operation_code;
  if (operation_code == "NEW_BUS") {
    q.type = QueryType::NewBus;
    is >> q.bus;
    int stop_count;
    is >> stop_count;
    q.stops.resize(stop_count);
    for (string& stop : q.stops) {
      is >> stop;
    }
  } else if (operation_code == "BUSES_FOR_STOP") {
    q.type = QueryType::BusesForStop;
    is >> q.stop;
  } else if (operation_code == "STOPS_FOR_BUS") {
    q.type = QueryType::StopsForBus;
    is >> q.bus;
  } else if (operation_code == "ALL_BUSES") {
    q.type = QueryType::AllBuses;
  } else {
    // TODO: What to do with invalid opcode?
  }
  return is;
}

struct BusesForStopResponse {
  // Наполните полями эту структуру
  string busesForStopResponse;
};

ostream& operator << (ostream& os, const BusesForStopResponse& r) {
  // Реализуйте эту функцию
  return os << r.busesForStopResponse;
}

struct StopsForBusResponse {
  // Наполните полями эту структуру
  vector<string> stopsForBusResponse;
};

ostream& operator << (ostream& os, const StopsForBusResponse& r) {
  // Реализуйте эту функцию
  return os << r.stopsForBusResponse;
}

struct AllBusesResponse {
  // Наполните полями эту структуру
  vector<string> allBusesResponse;
};

ostream& operator << (ostream& os, const AllBusesResponse& r) {
  // Реализуйте эту функцию
  return os << r.allBusesResponse;
}

class BusManager {
public:
  void AddBus(const string& bus, const vector<string>& stops) {
    // Реализуйте этот метод
    buses_to_stops[bus] = stops;
    for (const string& stop : stops) {
      stops_to_buses[stop].push_back(bus);
    }
  }

  BusesForStopResponse GetBusesForStop(const string& stop) const {
    // Реализуйте этот метод
    stringstream ss;
    if (stops_to_buses.count(stop) == 0) {
        ss << "No stop";
      } else {
        for (const string& bus : stops_to_buses.at(stop)) {
          ss << bus << " ";
        }
      }
      return {ss.str()};
  }

  StopsForBusResponse GetStopsForBus(const string& bus) const {
    // Реализуйте этот метод
    StopsForBusResponse r;
    stringstream ss;
    if (buses_to_stops.count(bus) == 0) {
      r.stopsForBusResponse.push_back("No bus");
    } else {
      for (const string& stop : buses_to_stops.at(bus)) {
        ss << "Stop " << stop << ": ";
        if (stops_to_buses.at(stop).size() == 1) {
          ss << "no interchange";
        } else {
          for (const string& other_bus : stops_to_buses.at(stop)) {
            if (bus != other_bus) {
              ss << other_bus << " ";
            }
          }
        }
        r.stopsForBusResponse.push_back(ss.str());
        ss.str("");
      }
    }
    return r;
  }

  AllBusesResponse GetAllBuses() const {
    // Реализуйте этот метод
    AllBusesResponse r;
    stringstream ss;
    if (buses_to_stops.empty()) {
      r.allBusesResponse.push_back("No buses");
    } else {
      for (const auto& bus_item : buses_to_stops) {
        ss << "Bus " << bus_item.first << ": ";
        for (const string& stop : bus_item.second) {
          ss << stop << " ";
        }
        r.allBusesResponse.push_back(ss.str());
        ss.str("");
      }
    }
    return r;
  }

  void TestAddBus(void) {
    {
      map<string, vector<string>> buses_to_stops_expected = {
        {"32", {"Tolstopaltsevo", "Marushkino", "Vnukovo"}}
      };
      map<string, vector<string>> stops_to_buses_expected = {
        {"Tolstopaltsevo", {"32"}},
        {"Marushkino", {"32"}},
        {"Vnukovo", {"32"}}
      };
      AddBus("32", {"Tolstopaltsevo", "Marushkino", "Vnukovo"});
      assert(buses_to_stops == buses_to_stops_expected);
      assert(stops_to_buses == stops_to_buses_expected);
    }
    {
      map<string, vector<string>> buses_to_stops_expected = {
        {"32", {"Tolstopaltsevo", "Marushkino", "Vnukovo"}},
        {"32K", {"Tolstopaltsevo", "Marushkino", "Vnukovo", "Peredelkino", "Solntsevo", "Skolkovo"}}
      };
      map<string, vector<string>> stops_to_buses_expected = {
        {"Tolstopaltsevo", {"32","32K"}},
        {"Marushkino", {"32","32K"}},
        {"Vnukovo", {"32","32K"}},
        {"Peredelkino", {"32K"}},
        {"Solntsevo", {"32K"}},
        {"Skolkovo", {"32K"}}
      };
      AddBus("32K", {"Tolstopaltsevo", "Marushkino", "Vnukovo", "Peredelkino", "Solntsevo", "Skolkovo"});
      assert(buses_to_stops == buses_to_stops_expected);
      assert(stops_to_buses == stops_to_buses_expected);
    }
    cout << "bm::TestAddBus OK" << endl;
  }

  void TestGetBusesForStop(void) {
    buses_to_stops = {
      {"32", {"Tolstopaltsevo", "Marushkino", "Vnukovo"}},
      {"32K", {"Tolstopaltsevo", "Marushkino", "Vnukovo", "Peredelkino", "Solntsevo", "Skolkovo"}}
    };
    stops_to_buses = {
      {"Tolstopaltsevo", {"32","32K"}},
      {"Marushkino", {"32","32K"}},
      {"Vnukovo", {"32","32K"}},
      {"Peredelkino", {"32K"}},
      {"Solntsevo", {"32K"}},
      {"Skolkovo", {"32K"}}
    };
    {
      BusesForStopResponse expected = {"No stop"};
      BusesForStopResponse result = GetBusesForStop("");
      assert(expected.busesForStopResponse == result.busesForStopResponse);
    }
    {
      BusesForStopResponse expected = {"No stop"};
      BusesForStopResponse result = GetBusesForStop("Novoebenevo");
      assert(expected.busesForStopResponse == result.busesForStopResponse);
    }
    {
      BusesForStopResponse expected = {"32 32K "};
      BusesForStopResponse result = GetBusesForStop("Vnukovo");
      assert(expected.busesForStopResponse == result.busesForStopResponse);
    }
    {
      BusesForStopResponse expected = {"32K "};
      BusesForStopResponse result = GetBusesForStop("Solntsevo");
      assert(expected.busesForStopResponse == result.busesForStopResponse);
    }
    {
      BusesForStopResponse expected = {"32K "};
      BusesForStopResponse result = GetBusesForStop("Peredelkino");
      assert(expected.busesForStopResponse == result.busesForStopResponse);
    }
    {
      BusesForStopResponse expected = {"32 32K "};
      BusesForStopResponse result = GetBusesForStop("Tolstopaltsevo");
      assert(expected.busesForStopResponse == result.busesForStopResponse);
    }
    cout << "bm::TestGetBusesForStop OK" << endl;
  }

  void TestGetStopsForBus(void) {
    buses_to_stops = {
      {"32", {"Tolstopaltsevo", "Marushkino", "Vnukovo"}},
      {"32K", {"Tolstopaltsevo", "Marushkino", "Vnukovo", "Peredelkino", "Solntsevo", "Skolkovo"}}
    };
    stops_to_buses = {
      {"Tolstopaltsevo", {"32","32K"}},
      {"Marushkino", {"32","32K"}},
      {"Vnukovo", {"32","32K"}},
      {"Peredelkino", {"32K"}},
      {"Solntsevo", {"32K"}},
      {"Skolkovo", {"32K"}}
    };

    {
      StopsForBusResponse expected = {
        {
          "Stop Tolstopaltsevo: 32K ",
          "Stop Marushkino: 32K ",
          "Stop Vnukovo: 32K "
        }
      };
      StopsForBusResponse output = GetStopsForBus("32");
      assert(expected.stopsForBusResponse == output.stopsForBusResponse);
    }
    {
      StopsForBusResponse expected = {
        {
          "Stop Tolstopaltsevo: 32 ",
          "Stop Marushkino: 32 ",
          "Stop Vnukovo: 32 ",
          "Stop Peredelkino: no interchange",
          "Stop Solntsevo: no interchange",
          "Stop Skolkovo: no interchange"
        }
      };
      StopsForBusResponse output = GetStopsForBus("32K");
      assert(expected.stopsForBusResponse == output.stopsForBusResponse);
    }
    {
      StopsForBusResponse expected = {
        {
          "No bus"
        }
      };
      StopsForBusResponse output = GetStopsForBus("666");
      assert(expected.stopsForBusResponse == output.stopsForBusResponse);
    }
    cout << "bm::TestGetStopsForBus OK" << endl;
  }

  void TestGetAllBuses(void) {
    {  
      buses_to_stops = {
        {"32", {"Tolstopaltsevo", "Marushkino", "Vnukovo"}},
        {"32K", {"Tolstopaltsevo", "Marushkino", "Vnukovo", "Peredelkino", "Solntsevo", "Skolkovo"}}
      };
      stops_to_buses = {
        {"Tolstopaltsevo", {"32","32K"}},
        {"Marushkino", {"32","32K"}},
        {"Vnukovo", {"32","32K"}},
        {"Peredelkino", {"32K"}},
        {"Solntsevo", {"32K"}},
        {"Skolkovo", {"32K"}}
      };
      
      AllBusesResponse expected = {
        {
          "Bus 32: Tolstopaltsevo Marushkino Vnukovo ",
          "Bus 32K: Tolstopaltsevo Marushkino Vnukovo Peredelkino Solntsevo Skolkovo "
        }
      };
      AllBusesResponse output = GetAllBuses();
      assert(expected.allBusesResponse == output.allBusesResponse);
    }

    {  
      buses_to_stops = {};
      
      AllBusesResponse expected = {
        {
          "No buses"
        }
      };
      AllBusesResponse output = GetAllBuses();
      assert(expected.allBusesResponse == output.allBusesResponse);
    }
    
    cout << "bm::TestGetAllBuses OK" << endl;
  }
private:
  map<string, vector<string>> buses_to_stops, stops_to_buses;
};

// Не меняя тела функции main, реализуйте функции и классы выше

void TestOperatorVector(void) {
  {
    stringstream ss;
    vector<string> input;
    string expected;
    ss << input;
    assert(ss.str() == expected);
  }
  {
    stringstream ss;
    vector<string> input = {"1"};
    string expected = "1";
    ss << input;
    assert(ss.str() == expected);
  }
  {
    stringstream ss;
    vector<string> input = {"1","2","3"};
    string expected = "1\n2\n3";
    ss << input;
    assert(ss.str() == expected);
  }
  cout << "TestOperatorVector OK" << endl;
}

void TestOperatorQuery(void) {
  {
    stringstream ss;
    Query input;
    Query expected = {
      .type = QueryType::AllBuses,
      .bus = {},
      .stop = {},
      .stops = {}
    };
    ss << "ALL_BUSES";
    ss >> input;
    assert(input == expected);
  }
  {
    stringstream ss;
    Query input;
    Query expected = {
      .type = QueryType::BusesForStop,
      .bus = {},
      .stop = {"Marushkino"},
      .stops = {}
    };
    ss << "BUSES_FOR_STOP Marushkino";
    ss >> input;
    assert(input == expected);
  }
  {
    stringstream ss;
    Query input;
    Query expected = {
      .type = QueryType::BusesForStop,
      .bus = {},
      .stop = {"Marushkino"},
      .stops = {}
    };
    ss << "BUSES_FOR_STOP Marushkino Novoebenevo";
    ss >> input;
    assert(input == expected);
  }
  {
    stringstream ss;
    Query input;
    Query expected = {
      .type = QueryType::StopsForBus,
      .bus = {"32K"},
      .stop = {},
      .stops = {}
    };
    ss << "STOPS_FOR_BUS 32K";
    ss >> input;
    assert(input == expected);
  }
  {
    stringstream ss;
    Query input;
    Query expected = {
      .type = QueryType::NewBus,
      .bus = {"32"},
      .stop = {},
      .stops = {{"Tolstopaltsevo", "Marushkino", "Vnukovo"}}
    };
    ss << "NEW_BUS 32 3 Tolstopaltsevo Marushkino Vnukovo";
    ss >> input;
    assert(input == expected);
  }
  {
    stringstream ss;
    Query input;
    Query expected = {
      .type = QueryType::AllBuses,
      .bus = {},
      .stop = {},
      .stops = {}
    };
    ss << "ALL_BUSES";
    ss >> input;
    assert(input == expected);
  }
  cout << "TestOperatorQuery OK" << endl;
}

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
