#include <iostream>
#include <vector>
#include <string>
using namespace std;

void PrintVector(const vector<string>& v) {
    for(auto s : v) {
        cout << s << endl;
    }
}

int main(void) {
    int n;
    cin >> n;
    vector<string> v(n);
    for (auto& s : v) {
        cin >> s;
    }
    PrintVector(v);
}