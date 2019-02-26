#pragma once
#include <string>
#include <iostream>
#include <vector>

using namespace std;

struct BusesForStopResponse {
  // Наполните полями эту структуру
  string busesForStopResponse;
};

ostream& operator << (ostream& os, const BusesForStopResponse& r);

struct StopsForBusResponse {
  // Наполните полями эту структуру
  vector<string> stopsForBusResponse;
};

ostream& operator << (ostream& os, const StopsForBusResponse& r);

struct AllBusesResponse {
  // Наполните полями эту структуру
  vector<string> allBusesResponse;
};

ostream& operator << (ostream& os, const AllBusesResponse& r);
