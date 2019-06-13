#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <future>
#include <queue>
using namespace std;

template <typename T>
class Synchronized {
public:
  explicit Synchronized(T initial = T()) : value(initial) {}

  struct Access {
    Access(T& value_, mutex& m_) : ref_to_value(value_), m(m_) {
      m.lock();
    }
    ~Access() {
      m.unlock();
    }
    T& ref_to_value;
  private:
    mutex& m;
  };

  Access GetAccess() {
    return {value,m};
  }
private:
  T value;
  mutex m;
};

class InvertedIndex {
public:
  // Can use InvertedIndex constructor to directly build
  // the index fro istream:
  // InvertedIndex(istream& document_input)
  void Add(string document);
  vector<pair<uint32_t,uint32_t>> Lookup(const string& word) const;

  const string& GetDocument(uint32_t id) const {
    return docs[id];
  }

  uint32_t GetDocsCount() const {
    return docs.size();
  }

private:
  const static vector<uint32_t> dummy;
  // Could write like this to get read of pair 
  // constructor and use string_view instead of string
  //
  // struct Entry {
  //   size_t docid, hitcount;
  // };
  // map<string_view, vector<Entry>> index;
  map<string, vector<pair<uint32_t,uint32_t>>> index;
  vector<string> docs;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBaseSingleThread(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStreamSingleThread(istream& query_input, ostream& search_results_output);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  InvertedIndex index;
  Synchronized<InvertedIndex> protected_index = Synchronized(index);
  queue<future<void>> queries_futures;
  queue<future<void>> update_futures;
  bool first_update = true;
};
