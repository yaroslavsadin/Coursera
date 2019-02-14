#include <iostream>
using namespace std;

int main() {
    int c = 5;
    int a = c++;
    int b = ++c;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    return 0;
}