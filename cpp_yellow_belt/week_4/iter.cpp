#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
#include <iterator>
using namespace std;

template<typename InputIt>
void PrintContainer(InputIt b, InputIt e) {
    for(InputIt i = b; i != e; i++) {
        cout << *i << " ";
    }
    cout << endl;
}

bool f(int a) {
    return (a > 5);
}

int main() {
    set<int> s = { 1,2,3,4,5,6,7,8,9 };
    vector<int> v;

    v.assign(begin(s), end(s));
    sort(begin(v), end(v),
        [](int lhs, int rhs) { return f(lhs) > f(rhs); });
    auto it = partition_point(begin(v), end(v), f);
    v.erase(begin(v), it);

    PrintContainer(v.begin(),v.end());

    return 0;
}
