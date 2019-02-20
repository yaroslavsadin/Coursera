#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

int main(void) {
    stringstream ss;
    ss << "lol " << "kek 1";
    cout << ss.str() << endl;
    return 0;
}