#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <list>

#include <string>
#include <chrono>

using namespace std;

struct Record {
  string id;
  string title;
  string user;
  int timestamp;
  int karma;

  bool operator==(const Record& r) const {
    return tie(id,title,user,timestamp,karma) == 
            tie(r.id,r.title,r.user,r.timestamp,r.karma);
  }
};

class RecordHasher {
  size_t operator()(const Record& r) const {
    const hash<string> shash;
    const hash<int> ihash;
#if 0
    size_t r1 = shash(r.id);
    size_t r2 = shash(r.title);
    size_t r3 = shash(r.user);
    size_t r4 = ihash(r.timestamp);
    size_t r5 = ihash(r.karma);

    size_t coeff = 45'253'941;

    return coeff * coeff * coeff * coeff * r1 +
            coeff * coeff * coeff * r2 +
            coeff * coeff * r3 +
            coeff * r4 + r5;
  #endif
    return shash(r.id);
  }
};

ostream& operator<<(ostream& os, const Record& r) {
    return os << '[' << r.id << ' ' << r.title << ' ' << r.user 
            << ' ' << r.timestamp << ' ' << r.karma << ']';
  }

template<typename Key, typename Value, typename Key1, typename Value1>
  bool EraseFromMultiMap(multimap<Key,Value>& mm, Key1 k, Value1 v) {
    auto its = mm.equal_range(k);
    for(auto it = its.first ;it != its.second; it++) {
      if (it->second->id == v) {
        mm.erase(it);
        return true;
      }
    }
    return false;
  }

template <class K, class V>
ostream& operator << (ostream& os, const multimap<K, V>& m) {
  os << "{";
  bool first = true;
  for (const auto& kv : m) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << kv.first << ": " << kv.second;
  }
  return os << "}";
}

// Реализуйте этот класс
class Database {
public:
  Database() {
    // cerr << "Database default ctor" << endl;
  }
  bool Put(const Record& record);
  const Record* GetById(const string& id) const;
  bool Erase(const string& id);

  template <typename Callback>
  void RangeByTimestamp(int low, int high, Callback callback) const;

  template <typename Callback>
  void RangeByKarma(int low, int high, Callback callback) const;

  template <typename Callback>
  void AllByUser(const string& user, Callback callback) const;
private:
  list<Record> records;
  unordered_map<string,list<Record>::iterator> id_to_rec_it;
  multimap<int,list<Record>::iterator> karma_to_rec_it;
  multimap<int,list<Record>::iterator> time_to_rec_it;
  multimap<string,list<Record>::iterator> user_to_rec_it;

  void Log(void) const {
    for(const auto& [id,it] : id_to_rec_it) {
      cerr << id <<": " << *it << endl;
    }
  }
};

bool Database::Put(const Record& record) {
  // cerr << "Put " << record << endl;
  if(id_to_rec_it.count(record.id)) {
    return false;
  } else {
    records.push_back(record);
    id_to_rec_it.insert(make_pair(records.back().id,prev(records.end())));
    karma_to_rec_it.insert(make_pair(records.back().karma,prev(records.end())));
    time_to_rec_it.insert(make_pair(records.back().timestamp,prev(records.end())));
    user_to_rec_it.insert(make_pair(records.back().user,prev(records.end())));
    return true;
  }
}

const Record* Database::GetById(const string& id) const {
  // cerr << "--------------> GetById " << id << endl;
  // Log();
  if(id_to_rec_it.count(id)) {
    // cerr << "RETURNED: " << *id_to_rec_it.at(id) << endl;
    return &*id_to_rec_it.at(id);
  } else {
    // cerr << "RETURNED nullptr" << endl;
    return nullptr;
  }
}

bool Database::Erase(const string& id) {
  // cerr << "Erase " << id << endl;
  if(id_to_rec_it.count(id)) {
    int karma = id_to_rec_it.at(id)->karma;
    int timestamp = id_to_rec_it.at(id)->timestamp;
    auto user = id_to_rec_it.at(id)->user;
    // EraseFromMultiMap(karma_to_rec_it, karma, id);
    // EraseFromMultiMap(time_to_rec_it, timestamp, id);
    // EraseFromMultiMap(user_to_rec_it, user, id);
    records.erase(id_to_rec_it.at(id));
    id_to_rec_it.erase(id);
    return true;
  } else {
    return false;
  }
}

template<typename Callback>
void Database::RangeByTimestamp(int low, int high, Callback callback) const {
  // cerr << "RangeByTimestamp " << low << " " << high << endl;
  // cerr << "-----------------------" << endl;
  // Log();
  // cerr << "-----------------------" << endl;
  auto it_begin = time_to_rec_it.lower_bound(low);
  if (it_begin == time_to_rec_it.end()) {
    return;
  }
  auto rec_it = it_begin->second;
  while(id_to_rec_it.count(rec_it->id) && (rec_it->timestamp <= high) && callback(*rec_it)) {
    if(++it_begin != time_to_rec_it.end())
      rec_it = it_begin->second;
    else
      break;
  }
}

template<typename Callback>
void Database::RangeByKarma(int low, int high, Callback callback) const {
  // cerr << "RangeByKarma " << low << " " << high << endl;
  auto it_begin = karma_to_rec_it.lower_bound(low);
  if (it_begin == karma_to_rec_it.end()) {
    return;
  }
  auto rec_it = it_begin->second;
  while(id_to_rec_it.count(rec_it->id) && (rec_it->karma <= high) && callback(*rec_it)) {
    if(++it_begin != karma_to_rec_it.end())
      rec_it = it_begin->second;
    else
      break;
  }
}

template <typename Callback>
void Database::AllByUser(const string& user, Callback callback) const {
  // cerr << "AllByUser " << user << endl;
  auto it_begin = user_to_rec_it.lower_bound(user);
  if (it_begin == user_to_rec_it.end()) {
    return;
  }
  auto rec_it = it_begin->second;
  while(it_begin != user_to_rec_it.end() && (it_begin->first == user) && callback(*rec_it)) {
    while(++it_begin != user_to_rec_it.end() && !id_to_rec_it.count(rec_it->id)) {
        rec_it = it_begin->second;
    }
  }
};

void TestRangeBoundaries() {
  const int good_karma = 1000;
  const int bad_karma = -10;

  Database db;
  db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
  db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

  int count = 0;
  db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestSameUser() {
  Database db;
  db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
  db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

  int count = 0;
  db.AllByUser("master", [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestReplacement() {
  const string final_body = "Feeling sad";

  Database db;
  db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
  db.Erase("id");
  db.Put({"id", final_body, "not-master", 1536107260, -10});

  auto record = db.GetById("id");
  ASSERT(record != nullptr);
  ASSERT_EQUAL(final_body, record->title);
}

void TestRangeByKarma() {
  Database db;
  db.Put({"id1", "upgrade1", "R2D2", 100500, 101});
  db.Put({"id2", "upgrade2", "R2D2", 100501, 102});
  db.Put({"id3", "upgrade3", "R2D2", 100502, 103});
  db.Put({"id4", "upgrade4", "R2D2", 100503, 104});
  db.AllByUser("Alex",[](const Record& r){return true;});
  db.RangeByKarma(106,106,[](const Record& r){return true;});
}

void TestErase() {
  Database db;
  
  db.Put({"id1", "upgrade1" ,"R2D2", 100500, 101});      // Put [id1 upgrade1 R2D2 100500 101]
  db.Put({"id2", "upgrade2" ,"R2D2", 100501, 102});      // Put [id2 upgrade2 R2D2 100501 102]
  db.Put({"id3", "upgrade3" ,"R2D2", 100502, 103});      // Put [id3 upgrade3 R2D2 100502 103]
  db.Put({"id4", "upgrade4" ,"R2D2", 100503, 104});      // Put [id4 upgrade4 R2D2 100503 104]
  db.GetById("unknown_id");      // GetById unknown_id
  db.GetById("md5");      // GetById md5
  db.AllByUser("Alex",[](const Record& r){return true;});      // AllByUser Alex
  db.RangeByKarma(106,106,[](const Record& r){return true;});      // RangeByKarma 106 106
  db.RangeByTimestamp(100504,100504,[](const Record& r){return true;});      // RangeByTimestamp 100504 100504
  db.Put({"md5", "something" ,"Alex", 100504, 106});      // Put [md5 something Alex 100504 106]
  db.GetById("unknown_id");      // GetById unknown_id
  auto rec = db.GetById("md5");      // GetById md5
  ASSERT(rec->id == "md5");
  db.AllByUser("Alex",[](const Record& r){return true;});      // AllByUser Alex
  db.RangeByKarma(106,106,[](const Record& r){return true;});      // RangeByKarma 106 106
  db.RangeByTimestamp(100504,100504,[](const Record& r){return true;});      // RangeByTimestamp 100504 100504
  db.Put({"md5", "something" ,"Alex", 100504, 106});      // Put [md5 something Alex 100504 106]
  db.GetById("unknown_id");      // GetById unknown_id
  db.GetById("md5");      // GetById md5
  db.AllByUser("Alex",[](const Record& r){return true;});      // AllByUser Alex
  db.RangeByKarma(106,106,[](const Record& r){return true;});      // RangeByKarma 106 106
  db.RangeByTimestamp(100504,100504,[](const Record& r){return true;});      // RangeByTimestamp 100504 100504
  db.GetById("md5");      // GetById md5
  db.Put({"id6", "upgrade2" ,"Alex", 100505, 103});      // Put [id6 upgrade2 Alex 100505 103]
  db.GetById("unknown_id");      // GetById unknown_id
  db.GetById("md5");     // GetById md5
  db.AllByUser("Alex",[](const Record& r){return true;});      // AllByUser Alex
  db.RangeByKarma(106,106,[](const Record& r){return true;});      // RangeByKarma 106 106
  db.RangeByTimestamp(100504,100504,[](const Record& r){return true;});      // RangeByTimestamp 100504 100504
  db.Erase("md5");      // Erase md5
        // { [id1 upgrade1 R2D2 100500 101]
        // [id2 upgrade2 R2D2 100501 102]
        // [id3 upgrade3 R2D2 100502 103]
        // [id4 upgrade4 R2D2 100503 104]
        // [md5 something Alex 100504 106]
        // [id6 upgrade2 Alex 100505 103]
        // }

}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestRangeBoundaries);
  RUN_TEST(tr, TestSameUser);
  RUN_TEST(tr, TestReplacement);
  RUN_TEST(tr, TestRangeByKarma);
  RUN_TEST(tr, TestErase);
  return 0;
}
