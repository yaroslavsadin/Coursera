#include <iostream>
#include <map>
using namespace std;

void PrintMap(const map<int,string> &m) {
    for(const auto& [key, value] : m) {
        cout << key << " " << value << endl;
    }
}

int main(void) {
    map<int, string> events;
    events[1950] = "Stroustrup birth";
    events[1941] = "Ritchie's birth";
    PrintMap(events);
    cout << events[1949] << endl;
    cout << events.size() << endl;
    return 0;
}