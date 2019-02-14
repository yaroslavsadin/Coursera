#include <iostream>
#include <string>
#include <cmath>
using namespace std;

int main(void) {
    int a,b;
    // while (1) {
    cin >> a >> b;
    while (a > 0 && b > 0) {
        if (a > b) {
            a %= b;
        } else {
            b %= a;
        }
    }
    cout << ((a > 0) ? a : b) << endl << endl;
    // }
    return 0;
}