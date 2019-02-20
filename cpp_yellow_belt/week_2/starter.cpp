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
        ss << "No stop" << endl;
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
      ss.clear();
      }
    }
    return r;
  }

  AllBusesResponse GetAllBuses() const {
    // Реализуйте этот метод
    AllBusesResponse r;
    stringstream ss;
    if (buses_to_stops.empty()) {
      ss << "No buses" << endl;
    } else {
      for (const auto& bus_item : buses_to_stops) {
        ss << "Bus " << bus_item.first << ": ";
        for (const string& stop : bus_item.second) {
          ss << stop << " ";
        }
        r.allBusesResponse.push_back(ss.str());
        ss.clear();
      }
    }
  }
public:
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

int main() {
#ifdef RUN_UNIT_TESTS
  TestOperatorVector();
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
