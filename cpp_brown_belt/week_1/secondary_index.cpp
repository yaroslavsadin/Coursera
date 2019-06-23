#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <string>
#include <chrono>

using namespace std;
using namespace chrono;

struct TotalDuration {
  string message;
  steady_clock::duration value;

  explicit TotalDuration(const string& msg);
  ~TotalDuration();
};

class AddDuration {
public:
  explicit AddDuration(steady_clock::duration& dest);
  explicit AddDuration(TotalDuration& dest);

  ~AddDuration();

private:
  steady_clock::duration& add_to;
  steady_clock::time_point start;
};

#define MY_UNIQ_ID_IMPL(lineno) _a_local_var_##lineno
#define MY_UNIQ_ID(lineno) MY_UNIQ_ID_IMPL(lineno)

#define ADD_DURATION(value) \
  AddDuration MY_UNIQ_ID(__LINE__){value};

TotalDuration::TotalDuration(const string& msg)
  : message(msg + ": ")
  , value(0)
{
}

TotalDuration::~TotalDuration() {
  ostringstream os;
  os << message
     << duration_cast<milliseconds>(value).count()
     << " ms" << endl;
  cerr << os.str();
}

AddDuration::AddDuration(steady_clock::duration& dest)
  : add_to(dest)
  , start(steady_clock::now())
{
}

AddDuration::AddDuration(TotalDuration& dest)
  : AddDuration(dest.value)
{
}

AddDuration::~AddDuration() {
  add_to += steady_clock::now() - start;
}

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

ostream& operator<<(ostream& os, const Record& r) {
    return os << '[' << r.id << ' ' << r.title << ' ' << r.user 
            << ' ' << r.timestamp << ' ' << r.karma << ']';
  }

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

template<typename Key, typename Value>
  bool EraseFromMultiMap(multimap<Key,Value>& mm, Key k, Value v) {
    auto it = mm.lower_bound(k);
    for(;it != mm.end(); it++) {
      if (it->second == v) {
        mm.erase(it);
        return true;
      }
    }
    return false;
  }

#if 0
template <class K, class V>
ostream& operator << (ostream& os, const unordered_map<K, V>& m) {
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
#endif

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
  Database() : put("Put"), erase("Erase"), range_time("RangeByTimestamp"),
  range_karma("RangeByKarma"), all_by_user("AllByUser"), get_by_id("GetById") {
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
  unordered_map<string, Record> id_to_record;
  multimap<int,string> timestamp_to_id;
  multimap<int,string> karma_to_id;
  multimap<string, string> user_to_id;

  TotalDuration put;
  TotalDuration erase;
  TotalDuration range_time;
  TotalDuration range_karma;
  TotalDuration all_by_user;
  TotalDuration get_by_id;
  
  void Log(void) const {
    cerr << "---------id_to_record-----------" << endl;
    cerr << id_to_record << endl;
    cerr << "---------timestamp_to_id-----------" << endl;
    cerr << timestamp_to_id << endl;
    cerr << "---------karma_to_id-----------" << endl;
    cerr << karma_to_id << endl;
    cerr << "---------user_to_id-----------" << endl;
    cerr << user_to_id << endl;
  }
};

bool Database::Put(const Record& record) {
  ADD_DURATION(put);
  // cerr << "Put" << endl;
  auto res = id_to_record.insert(make_pair(record.id,record));
  bool inserted = res.second;
  if(inserted) {
    timestamp_to_id.insert(make_pair(record.timestamp, record.id));
    karma_to_id.insert(make_pair(record.karma, record.id));
    user_to_id.insert(make_pair(record.user,record.id));
  }
  return inserted;
}

const Record* Database::GetById(const string& id) const {
  ADD_DURATION((TotalDuration&)get_by_id);
  // cerr << "GetById" << endl;
  if (id_to_record.count(id)) {
    return &id_to_record.at(id);
  } else {
    return nullptr;
  }
}

bool Database::Erase(const string& id) {
  ADD_DURATION(erase);
  // cerr << "Erase" << endl;
   if(id_to_record.count(id)) {
    int timestamp = id_to_record.at(id).timestamp;
    int karma = id_to_record.at(id).karma;
    string user = id_to_record.at(id).user;
    id_to_record.erase(id);
    EraseFromMultiMap(timestamp_to_id, timestamp, id);
    EraseFromMultiMap(karma_to_id, karma, id);
    EraseFromMultiMap(user_to_id, user, id);
    return true;
   } else {
     return false;
   }
}

template<typename Callback>
void Database::RangeByTimestamp(int low, int high, Callback callback) const {
  ADD_DURATION((TotalDuration&)range_time);
  // cerr << "RangeByTimestamp" << low << ' ' << high << endl;
  // Log();
  auto it_begin = timestamp_to_id.lower_bound(low);
  auto it_end = timestamp_to_id.upper_bound(high);
  if (it_begin == timestamp_to_id.end()) {
    return;
  }
  string id = it_begin->second;
  for(;it_begin != it_end && it_begin != timestamp_to_id.end() && callback(id_to_record.at(id)); it_begin++) {
    id = it_begin->second;
  }
}

template<typename Callback>
void Database::RangeByKarma(int low, int high, Callback callback) const {
  ADD_DURATION((TotalDuration&)range_karma);
  // cerr << "RangeByKarma" << low << ' ' << high << endl;
  // Log();
  auto it_begin = karma_to_id.lower_bound(low);
  auto it_end = karma_to_id.upper_bound(high);
  if (it_begin == karma_to_id.end()) {
    return;
  }
  string id = it_begin->second;
  for(;it_begin != it_end && it_begin != karma_to_id.end() && callback(id_to_record.at(id)); it_begin++) {
    id = it_begin->second;
  }
}

template <typename Callback>
void Database::AllByUser(const string& user, Callback callback) const {
  ADD_DURATION((TotalDuration&)all_by_user);
  // cerr << "AllByUser" << user << endl;
  // Log();
  auto it_begin = user_to_id.lower_bound(user);
  if (it_begin == user_to_id.end()) {
    return;
  }
  auto id = it_begin->second;
  for(auto it = it_begin; it != user_to_id.end() && it->first == user && callback(id_to_record.at(id)); it++) {
    id = it->second;
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

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestRangeBoundaries);
  RUN_TEST(tr, TestSameUser);
  RUN_TEST(tr, TestReplacement);
  RUN_TEST(tr, TestRangeByKarma);
  return 0;
}
