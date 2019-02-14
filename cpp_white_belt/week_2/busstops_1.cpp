#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

int main(void) {
    map <vector<string>,int> route_to_num;
    int cmd_cnt;
    cin >> cmd_cnt;

    for(int i = cmd_cnt; i > 0; i--) {
        int num_stops;
        cin >> num_stops;
        vector<string> stops(num_stops);

        for(auto& stop : stops) {
            cin >> stop;
        }

        if(route_to_num.count(stops)) {
            cout << "Already exists for " << route_to_num[stops] << endl;
        } else {
            int routes_cnt = route_to_num.size()+1;
            route_to_num[stops] = routes_cnt;
            cout << "New bus "  << routes_cnt << endl;
        }
    }

    return 0;
}