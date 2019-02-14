#include <iostream>
using namespace std;

int main(void) {
    string a,b,c;
    cin >> a >> b >> c;
    if (a < b && a < c) {
        cout << a;
        return 0;
    } else if (b < c) {
        cout << b;
        return 0;
    } else {
        cout << c;
    }
    return 0;
}