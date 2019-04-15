#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include "test_runner.h"
#include "profile.h"

using namespace std;

class Learner {
 private:
  vector<string> dict;

 public:
  int Learn(const vector<string>& words) {
    LOG_DURATION();
    int newWords = 0;
    for (const auto& word : words) {
      if (find(dict.begin(), dict.end(), word) == dict.end()) {
        ++newWords;
        dict.push_back(word);
      }
    }
    return newWords;
  }

  vector<string> KnownWords() {
    LOG_DURATION();
    sort(dict.begin(), dict.end());
    dict.erase(unique(dict.begin(), dict.end()), dict.end());
    return dict;
  }
};

int main() {
  Learner learner;
  string line;
  stringstream cin;
  for(int i = 0; i < 9999; i++) {
    cin << "kek" + to_string(i) << endl;
  }
  while (getline(cin, line)) {
    vector<string> words;
    stringstream ss(line);
    string word;
    while (ss >> word) {
      words.push_back(word);
    }
    cout << learner.Learn(words) << "\n";
  }
  cout << "=== known words ===\n";
  for (auto word : learner.KnownWords()) {
    cout << word << "\n";
  }
}