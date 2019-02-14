#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<int> Reversed(const vector<int>& v) {
    if (!v.size() || v.size() == 1) return v;
    vector<int> res;
    for(int i = v.size()-1; i >= 0; i--) {
        res.push_back(v[i]);
    }
    return res;
}

int main(void) {
    vector<int> source = {1, 5, 3, 4, 2};
    vector<int> res = Reversed(source);
    for(auto i : res) {
        cout << i << " ";
    }
    return 0;
}

