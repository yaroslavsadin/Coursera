#include "query.h"

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