#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>

/*
  document_input = DN = 50000
  document_size = DS = 1000
  num_unique_words = WN = 10000
  word_size = WS = 100
  query_inpt = QN = 500000
  max_query_words = QW = 10
*/

const vector<size_t> InvertedIndex::dummy = {};

// O(DS)
vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  // O(DS)
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

// O(DN*DS)
SearchServer::SearchServer(istream& document_input) {
  // O(DN*DS)
  UpdateDocumentBase(document_input);
}

template<typename T>
size_t VectorItToIdx(const vector<T>& v, T* it) {
  return it - v.data();
}

// O(DN*DS)
void SearchServer::UpdateDocumentBase(istream& document_input) {
  InvertedIndex new_index;

  // O(DN)
  for (string current_document; getline(document_input, current_document); ) {
    // O(DS)
    new_index.Add(move(current_document));
  }

  index = move(new_index);
}

void SearchServer::AddQueriesStream(
  istream& query_input, ostream& search_results_output
) {
  // O(QN*DNlogDN)
  for (string current_query; getline(query_input, current_query); ) {
    // O(QW)
    const auto words = SplitIntoWords(current_query);

    vector<pair<size_t,size_t>> docid_count(50000);
    // O(QW)
    for (const auto& word : words) {
      // O(logWN*logDN*WN)
      for (const size_t docid : index.Lookup(word)) {
        docid_count[docid].first = VectorItToIdx(docid_count,&docid_count[docid]);
        docid_count[docid].second++;
      }
    }

    // // O(DN)
    // vector<pair<size_t, size_t>> search_results(
    //   docid_count.begin(), docid_count.end()
    // );
    // O(DNlogDN)
    partial_sort(
      begin(docid_count),
      min(begin(docid_count)+5,end(docid_count)),
      end(docid_count),
      [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
        int64_t lhs_docid = lhs.first;
        auto lhs_hit_count = lhs.second;
        int64_t rhs_docid = rhs.first;
        auto rhs_hit_count = rhs.second;
        return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
      }
    );

    search_results_output << current_query << ':';
    for (auto [docid, hitcount] : Head(docid_count, 5)) {
      if(hitcount) {  
        search_results_output << " {"
          << "docid: " << docid << ", "
          << "hitcount: " << hitcount << '}';
      }
    }
    search_results_output << endl;
  }
}

// ~O(DS)
void InvertedIndex::Add(const string& document) {
  docs.push_back(document); // Amortized O(1)

  const size_t docid = docs.size() - 1;
  // O(DS)*AmortizedO(1)
  for (const auto& word : SplitIntoWords(document)) {
    index[word].push_back(docid);
  }
}

// O(DN*logWN)
const vector<size_t>& InvertedIndex::Lookup(const string& word) const {
  // O(logWN)
  if (auto it = index.find(word); it != index.end()) {
    // Here NRVO and copy elision don't work
    // So passing by reference is faster
    return it->second; // O(DN)
  } else {
    return {InvertedIndex::dummy};
  }
}
