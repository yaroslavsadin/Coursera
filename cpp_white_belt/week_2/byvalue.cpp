#include <iostream>
using namespace std;

void change_int(int x) {
    x = 42;
}

int main(void) {
    int a = 5;
    change_int(a);
    cout << a;
    return 0;
}