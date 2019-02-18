#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
#include <limits>
using namespace std;

int main(void) {
    cout << sizeof(long int) << endl;
    cout << numeric_limits<long int>::max() << " " <<
    numeric_limits<long int>::min();
    return 0;
}