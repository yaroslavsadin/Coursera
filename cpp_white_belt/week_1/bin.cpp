#include <iostream>
#include <vector>
using namespace std;

int main (void) {
    int a;
    vector<int> bin;
    cin >> a;

    while (a) {
        bin.push_back(a%2);
        a /= 2;
    }
    
    for(int i=bin.size()-1; i >= 0; i--) {
        cout << bin[i];
    }

    return 0;
}