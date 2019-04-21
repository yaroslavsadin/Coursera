#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
#include <tuple>
// #include "iterator_range.h"
#include "test_runner.h"
#include "profile.h"
using namespace std;

template <typename Iterator>
class IteratorRange {
public:
  IteratorRange(Iterator begin, Iterator end)
    : first(begin)
    , last(end)
    , size_(distance(first, last))
  {
  }

  Iterator begin() const {
    return first;
  }

  Iterator end() const {
    return last;
  }

  size_t size() const {
    return size_;
  }

private:
  Iterator first, last;
  size_t size_;
};

template <typename Iterator>
IteratorRange<Iterator> MakeRange(Iterator i1, Iterator i2) {
  return IteratorRange(i1,i2);
}

struct Record {
    set <int> clients;
    int rooms_total = 0;
};

class HotelManager {
public:
    HotelManager() {}
    // O(1)
    void Book(int64_t time, const string& hotel_name,
                        int client_id, int room_count) {
        data_[time][hotel_name].clients.insert(client_id);
        data_[time][hotel_name].rooms_total += room_count;
    }
    // O(D*Q*logQ)
    int Clients(const string& hotel_name) const {
        if(data_.empty()) {
            return 0;
        }
        set<int> clients;
        // O(D*Q)
        for(const auto& [time_,hotels] : GetPastDayRange()) {
            if(hotels.count(hotel_name)) {
                // O(Q)
                for(const auto& client : hotels.at(hotel_name).clients) {
                    clients.insert(client);
                }
            }
        }
        return clients.size();
    }
   // O(D*logQ)
    int Rooms(const string& hotel_name) const {
        if(data_.empty()) {
            return 0;
        }
        int num_rooms = 0;
        // O(D)
        for(const auto& [time_,hotels] : GetPastDayRange()) {
            if(hotels.count(hotel_name)) {
                // O(1)
                num_rooms += hotels.at(hotel_name).rooms_total;
            }
        }
        return num_rooms;
    }
private:
    map<int64_t,map<string,Record>> data_;
    // O(logQ)
    IteratorRange<map<int64_t,map<string,Record>>::const_iterator> GetPastDayRange(void) const {
        int64_t current_time = data_.rbegin()->first;
        // O(logQ)
        auto it = data_.upper_bound(current_time - 86400);
        return MakeRange(it,data_.end());
    }
};

int main(void) {
//   ios::sync_with_stdio(false);
//   cin.tie(nullptr);

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
      cout << manager.Clients(hotel_name) << endl;
    } else if (query_type == "ROOMS") {
      string hotel_name;
      cin >> hotel_name;
      cout << manager.Rooms(hotel_name) << endl;
    }
  }

  // O(D*Q*logQ)
  return 0;
}