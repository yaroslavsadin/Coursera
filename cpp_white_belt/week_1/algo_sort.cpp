#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

int main(void) {
    vector<int> s = {3,6,3,8,5,8,3,1};
    sort(begin(s),end(s));
    for(auto c : s)
        cout << c << ",";

    cout << endl;

    return 0;
}