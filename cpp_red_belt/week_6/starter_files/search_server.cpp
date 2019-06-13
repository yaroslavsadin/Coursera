#include "search_server.h"
#include "iterator_range.h"
#include "profile_advanced.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <cstdint>

/*
  document_input = DN = 50000
  document_size = DS = 1000
  num_unique_words = WN = 10000
  word_size = WS = 100
  query_inpt = QN = 500000
  max_query_words = QW = 10
*/

const vector<uint32_t> InvertedIndex::dummy = {};

// SplitIntoWordsView
// O(DS)
vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  // O(DS)
  return {
    make_move_iterator(istream_iterator<string>(words_input)),
    make_move_iterator(istream_iterator<string>())
  };
}

// O(DN*DS)
SearchServer::SearchServer(istream& document_input) {
  // O(DN*DS)
  UpdateDocumentBase(document_input);
}

template<typename T>
int64_t VectorPtrToIdx(const vector<T>& v, T* it) {
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

struct Pair {
  uint32_t first;
  uint32_t second;
};

Pair make_Pair(uint32_t a, uint32_t b) {
  return {a,b};
}

void SearchServer::AddQueriesStream(
  istream& query_input, ostream& search_results_output
) {
  TotalDuration internal_loop("internal_loop");
  TotalDuration partial_sort_("partial_sort");

  vector<Pair> docid_count;
  docid_count.reserve(index.GetDocsCount());
  // O(QN*QW*DN*logWN)
  for (string current_query; getline(query_input, current_query); ) {
    docid_count.clear();
    
    // O(QW*DN)
    for (const auto& word : SplitIntoWords(current_query)) {
      ADD_DURATION(internal_loop);
      // O(DN)
      for (const auto [docid,count] : index.Lookup(word)) { // logWN
        if (docid_count.size() <= docid) {
          docid_count.resize(docid+1);
        }
        docid_count[docid].first = docid;
        docid_count[docid].second += count;
      }
    }

    // // O(DN)
    // vector<pair<uint32_t, uint32_t>> search_results(
    //   docid_count.begin(), docid_count.end()
    // );
    // O(DN)
    {
      ADD_DURATION(partial_sort_);  
      partial_sort(
        begin(docid_count),
        min(begin(docid_count)+5,end(docid_count)),
        end(docid_count),
        [&](const Pair& lhs, const Pair& rhs) {
          int64_t lhs_docid = lhs.first;
          auto lhs_hit_count = lhs.second;
          int64_t rhs_docid = rhs.first;
          auto rhs_hit_count = rhs.second;
          return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
        }
      );
    }
// #define search_results_output cout
    search_results_output << current_query << ':';
    for (auto [docid, hitcount] : Head(docid_count, 5)) {
      if(hitcount) {
        search_results_output << " {"
          << "docid: " << docid << ", "
          << "hitcount: " << hitcount << '}';
      }
    }
    search_results_output << '\n';
  }
}

// ~O(DS)
void InvertedIndex::Add(string document) {
  docs.push_back(move(document)); // Amortized O(1)
  const uint32_t docid = docs.size() - 1;
  // O(DS)*AmortizedO(1)
  for (auto word : SplitIntoWords(docs.back())) {
    vector<pair<uint32_t,uint32_t>>& docs = index[word];
    // It is enough to check docs.back() to see if the word already
    // has the document counted:
    // if (!docids.empty() && docids.back().docid == docid) {
    //     ++docids.back().hitcount;
    //   } else {
    //     docids.push_back({docid, 1});
    //   }
    auto it = find_if(docs.begin(),docs.end(),[docid](const pair<size_t,size_t> p) {
      return (p.first == docid);
    });
    if(it == docs.end()) {
      docs.push_back({docid,1});
    } else {
      it->second++;
    }
  }
}

// O(logWN)
vector<pair<uint32_t,uint32_t>> InvertedIndex::Lookup(const string& word) const {
  // O(logWN)
  if (auto it = index.find(word); it != index.end()) {
    // Here NRVO and copy elision don't work
    // So passing by reference is faster
    return it->second;
  } else {
    return {};
  }
}
