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
    auto range_middle = elements.begin() + elements.size() / 2;

    MergeSort(elements.begin(), range_middle);
    MergeSort(range_middle, elements.end());
    merge(elements.begin(),range_middle,
            range_middle,elements.end(),
            range_begin);
}

int main(void) {
    vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1};
    MergeSort(begin(v), end(v));
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;

    return 0;
}