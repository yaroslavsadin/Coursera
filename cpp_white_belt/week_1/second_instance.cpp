#include <iostream>
#include <string>
using namespace std;

int main(void) {
    string a;
    cin >> a;

    int output = -2;
    for(int i = 0; i < a.size(); i++) {
        if (a[i] == 'f') {
            if (output == -2) {
                output = -1;
            } else if (output == -1) {
                output = i;
                break;
            }
        }
    }
    cout << output;
    return 0;
}