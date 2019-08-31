#include <iostream>
#include <vector>
#include <algorithm>
#include "test_runner.h"

using namespace std;

template<typename RandomIt>
void sort_(const RandomIt begin_, const RandomIt end_) {
    sort(begin_++,end_);
}

int main(void) {
    vector<int> nums{4,5,6,2,8,4,0};
    sort_(nums.begin(), nums.end());
    const vector<int>::iterator it = nums.begin();
    *it = 0;
    cout << nums << endl;
    return 0;
}