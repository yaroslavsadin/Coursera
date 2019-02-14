#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

struct Duration {
    int hours;
    int mins;
};

istream& operator >>(istream& stream, Duration& dur) {
    stream >> dur.hours;
    stream.ignore(1);
    stream >> dur.mins;
    return stream;
}

ostream& operator <<(ostream& stream, const Duration& dur) {
    stream << dur.hours << ":" << dur.mins;
    return stream;
}

int main(void) {
    Duration dur1,dur2,dur3;
    cin >> dur1 >> dur2 >> dur3;
    cout << dur1 << endl << dur2 << endl << dur3 << endl;
    return 0;
}