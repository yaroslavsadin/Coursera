#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool IsPalindrom(string n) {
    if (n.size() == 0) return true;
    for(int i=0,j=n.size()-1; i <= j; i++, j--) {
        char a = n[i];
        char b = n[j];
        if (a != b) return false;
    }
    return true;
}

int main(void) {
    string a;
    cin >> a;
    cout << IsPalindrom(a);
    return 0;
}
