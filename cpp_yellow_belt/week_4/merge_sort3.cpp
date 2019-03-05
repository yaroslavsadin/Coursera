#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if (range_end - range_begin < 2) return;
    vector<typename RandomIt::value_type> elements(range_begin, range_end);
    auto range_middle1 = elements.begin() + elements.size() / 3;
    auto range_middle2 = elements.begin() + (elements.size() / 3) * 2;

    MergeSort(elements.begin(), range_middle1);
    MergeSort(range_middle1, range_middle2);
    vector<typename RandomIt::value_type> elements1;
    merge(elements.begin(),range_middle1,
            range_middle1,range_middle2,
            back_inserter(elements1));
    MergeSort(range_middle2, elements.end());
    merge(elements1.begin(),elements1.end(),
            range_middle2,elements.end(),
            range_begin);
}

int main(void) {
    vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1, 5};
  MergeSort(begin(v), end(v));
  for (int x : v) {
    cout << x << " ";
  }
  cout << endl;
  return 0;
}