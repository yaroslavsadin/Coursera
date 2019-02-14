#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

int main(void) {
    int N;
    cin >> N;
    vector<int> nums(N);
    for(auto& e : nums) {
        cin >> e;
    }
    sort (begin(nums), end(nums), [](int a, int b) {return abs(a) < abs(b);});
    for(const auto& e : nums) {
        cout << e << " ";
    }
    return 0;
}