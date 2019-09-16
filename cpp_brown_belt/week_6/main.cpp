#include <iostream>
#include "test_runner.h"
#include "database.h"
#include "misc.h"

using namespace std;

int main(void) {
    BusDB db;
    db.AddStop("hell1",234, 556);
    db.AddStop("hell2",654, 466);
    db.AddStop("hell3", 423, 23423);
    db.AddStop("hell1", 234, 556);
    vector<string> stops{"hell1", "hell2", "hell3", "hell1"};
    db.AddBus(666,Range(stops.begin(),stops.end()));
    if(auto res = db.BusQuery(6667); res.has_value()) {
        cout << res.value()->unique_stops << endl;
        cout << res.value()->stops << endl;
        cout << res.value()->route_length << endl;
        for(const auto it : res.value()->route) {
            cout << it->first << endl;
        }
    } else {
        cout << "No bus found\n";
    }
}