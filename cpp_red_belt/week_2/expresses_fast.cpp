#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <set>
#include "profile.h"
#include <stdexcept>

using namespace std;

class RouteManager {
public:
  void AddRoute(int start, int finish) {       // O(logQ)
    reachable_lists_[start].insert(finish); // O(logQ)
    reachable_lists_[finish].insert(start); // O(logQ)
  }
  int FindNearestFinish(int start, int finish) const { // O(logQ)
    int result = abs(start - finish);
    if (reachable_lists_.count(start) < 1) { //O(logQ)
        return result;
    }
    const set<int>& reachable_stations = reachable_lists_.at(start); //O(logQ)
    if (!reachable_stations.empty()) {
      // O(logQ)
      auto temp = reachable_stations.lower_bound(finish);
      int distance = result;
      if (temp == reachable_stations.begin()) {
        distance = abs(finish - *temp);
      } else if (temp == reachable_stations.end()) {
        distance = abs(finish - *prev(temp));
      } else {
        distance = min(abs(finish - *prev(temp)),abs(finish - *temp));
      }
      result = min( result, distance );
    }
    return result;
  }
private:
  map<int, set<int>> reachable_lists_;
};


int main() {
  RouteManager routes;
  // ifstream cin{"C:\\Work\\Coursera\\cpp_red_belt\\week_2\\slow.txt"};
  // if(!cin) {
  //   throw invalid_argument("Not opened");
  // }

  int query_count;
  cin >> query_count;

  LOG_DURATION("All");
  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int start, finish;
    cin >> start >> finish;
    if (query_type == "ADD") {
      routes.AddRoute(start, finish); // O(logQ)
    } else if (query_type == "GO") {
      cout << routes.FindNearestFinish(start, finish) << endl;
      // routes.FindNearestFinish(start, finish) ; // O(logQ)
    }
  }

  // O(QlogQ)
  return 0;
}
