#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
#include <queue>
#include "test_runner.h"
#include "profile.h"
using namespace std;

struct Event {
  int64_t timestamp;
  string hotel_name;
  int client_id;
  int room_count;
};

class HotelManager {
public:
    HotelManager() {}
    // O(LlogQ)
    void Book(int64_t time, const string& hotel_name,
                        int client_id, int room_count) {
      Adjust(time);
      hotel_to_unique_customers[hotel_name][client_id]++;
      hotel_to_rooms[hotel_name] += room_count;
      events.push({
        .timestamp = time,
        .hotel_name = hotel_name,
        .client_id = client_id,
        .room_count = room_count
      });
    }

    // O(LlogQ)
    size_t Clients(const string& hotel_name) const {
      if(events.empty() || !hotel_to_unique_customers.count(hotel_name)) {
        return 0;
      }
      return hotel_to_unique_customers.at(hotel_name).size();
    }

    // O(LlogQ)
    int Rooms(const string& hotel_name) const {
      if(events.empty() || !hotel_to_unique_customers.count(hotel_name)) {
        return 0;
      }
      return hotel_to_rooms.at(hotel_name);
    }
private:
  queue<Event> events;
  map<string,map<int,int>> hotel_to_unique_customers;
  map<string,int> hotel_to_rooms;
  // amortized O(LlogQ)
  void Adjust(int64_t time) {
    while (!events.empty() && events.front().timestamp <= time - 86400) {
      hotel_to_unique_customers[events.front().hotel_name][events.front().client_id]--;
      if(!hotel_to_unique_customers[events.front().hotel_name][events.front().client_id]) {
        hotel_to_unique_customers[events.front().hotel_name].erase(events.front().client_id);
      }
      hotel_to_rooms[events.front().hotel_name] -= events.front().room_count;
      events.pop();
    }
  }
};

int main(void) {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  TestRunner tr;

#if 0
  ifstream oss{"C:\\Work\\Coursera\\cpp_red_belt\\week_2\\hotels.txt"};
  cin.rdbuf(oss.rdbuf());
#endif

  HotelManager manager;

  int query_count;
  cin >> query_count;

  LOG_DURATION();
  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;

    if (query_type == "BOOK") {
      int64_t time;
      string hotel_name;
      int client_id, room_count;
      cin >> time >> hotel_name >> client_id >> room_count;
      manager.Book(time,hotel_name,client_id,room_count);
    } else if (query_type == "CLIENTS") {
      string hotel_name;
      cin >> hotel_name;
      cout << manager.Clients(hotel_name) << '\n';
    } else if (query_type == "ROOMS") {
      string hotel_name;
      cin >> hotel_name;
      cout << manager.Rooms(hotel_name) << '\n';
    }
  }

  // O(Q*L*logQ)
  return 0;
}