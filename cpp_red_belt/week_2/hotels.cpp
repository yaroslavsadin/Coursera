#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
#include <tuple>
#include "iterator_range.h"
#include "test_runner.h"
#include "profile.h"
using namespace std;

struct Record {
    int client_id;
    int num_rooms;
};

class HotelManager {
public:
    HotelManager() {}
    void Book(uint64_t time, const string& hotel_name,
                        int client_id, int room_count) {
        data_[time][hotel_name] = {
            .client_id = client_id, 
            .num_rooms = room_count
        };
    }
    int Clients(const string& hotel_name) const {
        if(data_.empty()) {
            return 0;
        }
        set<int> clients;
        for(const auto& [key,value] : GetPastDayRange(hotel_name)) {
            if(value.count(hotel_name)) {
                clients.insert(value.at(hotel_name).client_id);
            }
        }
        return clients.size();
    }
    int Rooms(const string& hotel_name) const {
        if(data_.empty()) {
            return 0;
        }
        int num_rooms;
        for(const auto& [key,value] : GetPastDayRange(hotel_name)) {
            if(value.count(hotel_name)) {
                num_rooms += value.at(hotel_name).num_rooms;
            }
        }
        return num_rooms;
    }
private:
    map<uint64_t,map<string,Record>> data_;
    IteratorRange<map<uint64_t, map<string,Record>>::const_iterator> GetPastDayRange(const string& hotel_name) const {
        uint64_t current_time = data_.rbegin()->first;
        if(current_time < 86400) {
            return MakeRange(data_.begin(),data_.end());
        } else {
            auto it = data_.upper_bound(current_time - 86400);
            return MakeRange(it,data_.end());
        }
    }
};

int main(void) {
//   ios::sync_with_stdio(false);
//   cin.tie(nullptr);

//   TestRunner tr;

#if 1
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
      uint64_t time;
      string hotel_name;
      int client_id, room_count;
      cin >> time >> hotel_name >> client_id >> room_count;
      manager.Book(time,hotel_name,client_id,room_count);
    } else if (query_type == "CLIENTS") {
      string hotel_name;
      cin >> hotel_name;
      cout << manager.Clients(hotel_name) << endl;
    } else if (query_type == "ROOMS") {
      string hotel_name;
      cin >> hotel_name;
      cout << manager.Rooms(hotel_name) << endl;
    }
  }

  // O(L*Q)
  return 0;
}