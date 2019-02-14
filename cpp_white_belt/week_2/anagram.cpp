#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

map<char,int> BuildDict(string word) {
    map<char,int> res;
    for(auto c : word) {
        res[c]++;
    }
    return res;
}

int main(void) {
    int n;
    cin >> n;

    for(int i = n; i > 0; i--) {
        string first;
        string second;

        cin >> first >> second;

        map<char,int> count1 = BuildDict(first);
        map<char,int> count2 = BuildDict(second);

        if (count1==count2) {
            cout << "YES" << endl;
        } else {
            cout << "NO" << endl;
        }
    }

    return 0;
}