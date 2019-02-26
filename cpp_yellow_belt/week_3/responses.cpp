#include "responses.h"

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

ostream& operator << (ostream& os, const BusesForStopResponse& r) {
  // Реализуйте эту функцию
  return os << r.busesForStopResponse;
}

ostream& operator << (ostream& os, const StopsForBusResponse& r) {
  // Реализуйте эту функцию
  return os << r.stopsForBusResponse;
}

ostream& operator << (ostream& os, const AllBusesResponse& r) {
  // Реализуйте эту функцию
  return os << r.allBusesResponse;
}
