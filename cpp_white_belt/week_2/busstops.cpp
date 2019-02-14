#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

int main(void) {
    vector<string> buses;
    // Alternatevly use two dicts, one for bus_to_stop and the second for stop_to_bus
    map<string,vector<string>> bus_to_stops;

    int cmd_cnt;
    cin >> cmd_cnt;

    for(int i = cmd_cnt; i > 0; i--) {
        string opcode;
        cin >> opcode;

        if(opcode == "NEW_BUS") {
            string bus;
            vector<string> stops; // Can use resize method instead of creating temproary vector:
            // vector<string>& stops = buses_to_stops[bus];
            // stops.resize(stop_count);
            int stop_count;

            cin >> bus >> stop_count;
            for(;stop_count > 0;stop_count--) {
                string stop;
                cin >> stop;
                stops.push_back(stop);
            }
            buses.push_back(bus);
            bus_to_stops[bus] = stops;
        } else if (opcode == "BUSES_FOR_STOP") {
            vector<string> buses_ordered;
            string stop;
            cin >> stop;
            for(const auto& bus : buses) {
                if(find(bus_to_stops[bus].begin(), bus_to_stops[bus].end(), stop) != bus_to_stops[bus].end()) {
                    buses_ordered.push_back(bus);
                }
            }
            if(!buses_ordered.size()) {
                cout << "No stop" << endl;
            } else {
                for (const auto &bus : buses_ordered) {
                    cout << bus << " ";
                }
                cout << endl;
            }
        } else if(opcode == "STOPS_FOR_BUS") {
            string bus;
            cin >> bus;

            if(!bus_to_stops.count(bus)) {
                cout << "No bus" << endl;
            } else {
                vector<string> stop_to_buses;
            
                for(auto stop : bus_to_stops[bus]) {
                    for(auto b : buses) {
                        if (b == bus) continue;
                        if(find(bus_to_stops[b].begin(), bus_to_stops[b].end(), stop) != bus_to_stops[b].end()) {
                            stop_to_buses.push_back(b);
                        }
                    }
                    if (!stop_to_buses.size()) {
                        cout << "Stop " << stop << ": no interchange" << endl;
                    } else {
                        cout << "Stop " << stop << ": ";
                        for(auto i : stop_to_buses) {
                            cout << i << " ";
                        }
                        cout << endl;       
                    }
                    stop_to_buses.resize(0);
                }
            }
        } else if(opcode == "ALL_BUSES") {
            if(bus_to_stops.empty()) {
                cout << "No buses" << endl;
            }
            for(auto b : bus_to_stops) {
                cout << "Bus " << b.first << ":" << " ";
                for(auto i : b.second) {
                    cout << i << " ";
                }
                cout << endl;
            }
        }
    }

    return 0;
}