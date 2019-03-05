#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
using namespace std;

int main(void) {
    vector<int> a = {1,1,2,2,2,3,5,4,6,6,7};
    int b = binary_search(a.begin(),a.end(),4);
    cout << b << endl;
    auto it = equal_range(a.begin(),a.end(),2);
    for(auto i = it.first; i != it.second; i++) {
        cout << *i << endl;
    }
    return 0;
}