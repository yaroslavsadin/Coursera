#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
#include <tuple>
using namespace std;

int main(void) {
    tuple t(7, "C++", true);
    cout << get<1>(t) << endl;
    pair p("lol",666);
    auto [s,i] = p;
    cout << s << i << endl;
    return 0;
}