#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
using namespace std;

vector<string> SplitIntoWords(const string& s) {
    vector<string> res;
    auto f = s.begin();
    while(f < s.end()) {
        auto l = find(f,s.end(),' ');
        res.push_back({f,l});
        f = ++l;
    }
    return res;
}

int main() {
  string s = "C Cpp Java Python";

  vector<string> words = SplitIntoWords(s);
  cout << words.size() << " ";
  for (auto it = begin(words); it != end(words); ++it) {
    if (it != begin(words)) {
      cout << "/";
    }
    cout << *it;
  }
  cout << endl;
  
  return 0;
}
