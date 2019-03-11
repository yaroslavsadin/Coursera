#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
// #include "test_framework.h"
// #include "LowerUpperTests.h"
// #include "LowerUpper.h"
using namespace std;

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(
    RandomIt range_begin, RandomIt range_end,
    char prefix) {
        string a;
        a += prefix;
        auto ans = equal_range(range_begin,range_end,a,[] (const string& s1, const string& s2 ) {
            return s1[0] < s2[0];
        });
        return ans;
    }

int main() {
    // {
    //     TestRunner t;
    //     t.RunTest(TestLowerBound,"TestLowerBound");
    //     t.RunTest(TestUpperBound,"TestUpperBound");
    // }

  const vector<string> sorted_strings = {"moscow", "murmansk", "vologda"};
  
  const auto m_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), 'm');
  for (auto it = m_result.first; it != m_result.second; ++it) {
    cout << *it << " ";
  }
  cout << endl;
  
  const auto p_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), 'p');
  cout << (p_result.first - begin(sorted_strings)) << " " <<
      (p_result.second - begin(sorted_strings)) << endl;
  
  const auto z_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), 'z');
  cout << (z_result.first - begin(sorted_strings)) << " " <<
      (z_result.second - begin(sorted_strings)) << endl;
  
  return 0;
}