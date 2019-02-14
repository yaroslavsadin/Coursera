#include <iostream>
#include <string>
#include <vector>
using namespace std;

int Factorial(int n) {
    if (n < 0)
        return 1;
    
    if (n <= 1)
        return 1;
    else    
        return n * Factorial(n-1);
}

int main(void) {
    int a;
    cin >> a;
    cout << Factorial(a);
    return 0;
}
