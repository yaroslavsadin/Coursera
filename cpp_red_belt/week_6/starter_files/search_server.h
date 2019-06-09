#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>
using namespace std;

class InvertedIndex {
public:
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
  map<string, vector<pair<uint32_t,uint32_t>>> index;
  vector<string> docs;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  InvertedIndex index;
};
