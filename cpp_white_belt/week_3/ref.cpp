#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

int a(0);

int& GetVar(void) {
    return a;
}

int main(void) {
    vector<int> b {};
    a = 5;
    cout << a << endl;
    GetVar() = 6;
    cout << a << endl;
    for(auto i : b)
        cout << i << " ";
    return 0;
}