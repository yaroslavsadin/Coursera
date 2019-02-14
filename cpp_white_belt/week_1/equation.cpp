#include <iostream>
#include <cmath>
using namespace std;

int main(void) {
    double a,b,c;
    double d;
    cin >> a >> b >> c;
    d = b*b - 4*a*c;

    if (a == 0 && b == 0) return 0;

    if (a == 0) {
        cout << (-c)/b;
    } else if (d == 0) {
        cout << -b/(2*a);
    } else if (d > 0) {
        double x1 = (-b + sqrt(d))/(2*a); 
        double x2 = (-b - sqrt(d))/(2*a); 
        cout << x1 << " " << x2;
    }
    return 0;
}