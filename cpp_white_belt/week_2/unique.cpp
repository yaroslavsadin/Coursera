#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

int main(void) {
    set<string> unique;
    int cnt;
    cin >> cnt;

    for(;cnt > 0;cnt--) {
        string a;
        cin >> a;
        unique.insert(a);
    }
    cout << unique.size();
    return 0;
}