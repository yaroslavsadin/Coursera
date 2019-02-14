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
    vector<string> strings(N);
    for(auto& e : strings) {
        cin >> e;
    }
    sort (begin(strings), end(strings), [](string a, string b) {
        for(auto& c : a) {
            c = tolower(c);
        }
        for(auto& c : b) {
            c = tolower(c);
        }
        return a < b;
    });
    for(const auto& e : strings) {
        cout << e << " ";
    }
    return 0;
}