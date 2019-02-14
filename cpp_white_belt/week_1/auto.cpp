#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main(void) {
    vector<string> s = {"a","b","c"};

    for(auto c : s)
        cout << c << ",";

    cout << endl;

    return 0;
}