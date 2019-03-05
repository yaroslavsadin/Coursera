#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
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

int main(void) {
    int a;
    cin >> a;
    vector<int> v(a);
    for(auto &i : v) {
        i = a--;
    }
    PrintContainer(v.begin(),v.end());
    while(prev_permutation(v.begin(),v.end())) {
        PrintContainer(v.begin(),v.end());
    }
}