#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <future>
#include <functional>
#include <list>
using namespace std;

template <typename Iterator>
class Page {
public:
  Page(Iterator b, Iterator e)
  : first(b), second(e) {}
  Iterator begin() const {
    return first;
  }
  Iterator end() const {
    return second;
  }
  size_t size() const {
    return second - first;
  }
private:
  Iterator first;
  Iterator second;
};

// Реализуйте шаблон класса Paginator

template <typename Iterator>
class Paginator {
public:
  Paginator(Iterator b, Iterator e, size_t page_size) {
    Iterator current = b;
    while(e - current >= page_size) {
      pages.push_back({current,current+page_size});
      current = next(current,page_size);
    }
    if(current != e) {
      pages.push_back({current,e});
    }
  }
  // auto  = typename vector<Page<Iterator>>::const_iterator
  const auto begin() const {
    return pages.begin();
  }
  const auto end() const {
    return pages.end();
  }
  size_t size() const {
    return pages.size();
  }
private:
  vector<Page<Iterator>> pages;
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
  // Реализуйте этот шаблон функции
  return Paginator(c.begin(),c.end(),page_size);
}

struct Stats {
  map<string, int> word_frequences;

  void operator += (const Stats& other) {
    for(const auto [word,freq] : other.word_frequences) {
      word_frequences[word] += freq;
    }
  }
};

string GetNextWord(string_view& sv) {
  size_t pos = sv.find(' ');
  auto res = sv.substr(0,pos);
  sv.remove_prefix(pos != sv.npos ? pos+1 : sv.size());
  return string(res);
}

Stats ExploreLine(const set<string>& key_words, const string& line) {
  string_view sv(line);
  Stats res;
  while(sv.size()) {
    auto word = GetNextWord(sv);
    if(key_words.count(word)) {
      res.word_frequences[move(word)]++;
    }
  }
  return res;
}

Stats ExploreKeyWordsSingleThread(
  const set<string>& key_words, istream& input
) {
  Stats result;
  for (string line; getline(input, line); ) {
    result += ExploreLine(key_words, line);
  }
  return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
  // Реализуйте эту функцию
  Stats result;
  vector<future<Stats>> stats_;
  list<stringstream> vss;
  const int page_size = 1500;
  while(input) {
    string line;
    vss.emplace_back();
    for(int i = 0; i < page_size; i++) {
      getline(input, line);
      vss.back() << line << "\n";
      if(!input) {
        break;
      }
    }
    stats_.push_back(async(ExploreKeyWordsSingleThread,key_words,ref(vss.back())));
  }
  
  for(auto& stat : stats_) {
    result += stat.get();
  }
  return result;
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}
