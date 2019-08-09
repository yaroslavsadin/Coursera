#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
#include <queue>
#include <unordered_set>
#include "test_runner.h"
#include "profile.h"
using namespace std;

struct Event {
  int64_t timestamp;
  string hotel_name;
  int client_id;
  int room_count;
};

constexpr int SECS_IN_DAY {86400};

class HotelManager {
private:
  struct Record {
    int64_t time;
    string hotel_name;
    int client_id;
    int room_count;
  };
  void Adjust() {
    if (records_.empty())
      return;
    while(records_.front().time <= current_time - SECS_IN_DAY) {
        string& hotel_name = records_.front().hotel_name;
        int client_id = records_.front().client_id;
        int room_count = records_.front().room_count;

        hotel_to_client_room_counts[hotel_name][client_id] -= room_count;
        if(!hotel_to_client_room_counts[hotel_name][client_id]) {
          hotel_to_client_room_counts[hotel_name].erase(client_id);
        }
        hotel_to_num_rooms[hotel_name] -= room_count;
        records_.pop();
    }
  }
public:
    HotelManager() {}
    // O(LlogQ)
    void Book(int64_t time, const string& hotel_name,
                        int client_id, int room_count) {
      current_time = time;
      records_.push({time,hotel_name,client_id,room_count});
      hotel_to_client_room_counts[records_.back().hotel_name][client_id] += room_count;
      hotel_to_num_rooms[records_.back().hotel_name] += room_count;
      Adjust();
    }

    // O(LlogQ)
    size_t Clients(const string& hotel_name) const {
      if(hotel_to_client_room_counts.count(hotel_name)) {
        return hotel_to_client_room_counts.at(hotel_name).size();
      }
      return 0;
    }

    // O(LlogQ)
    int Rooms(const string& hotel_name) const {
      if(hotel_to_client_room_counts.count(hotel_name)) {
        return hotel_to_num_rooms.at(hotel_name);
      }
      return 0;
    }
private:
  queue<Record> records_;
  unordered_map< string, unordered_map<int, int> > hotel_to_client_room_counts;
  unordered_map<string,int> hotel_to_num_rooms;
  int64_t current_time;
};

void Test() {
}

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

  LOG_DURATION("");
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