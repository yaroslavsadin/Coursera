#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iomanip>
using namespace std;

int main(void) {
    ifstream input("input.txt");
    double n;
    while(input >> n) {
        cout << fixed << setprecision(3) << n << endl;
    }

    return 0;
}