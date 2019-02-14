#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

int main(void) {
    map<string,string> capitals;

    int cmd_cnt;
    cin >> cmd_cnt;

    for(int i = cmd_cnt; i > 0; i--) {
        string opcode;
        cin >> opcode;

        if(opcode == "CHANGE_CAPITAL") {
            string country, new_capital;
            cin >> country >> new_capital;
            if(!capitals.count(country))
                cout << "Introduce new country " << country << " with capital " << new_capital << endl;
            else if (capitals[country] == new_capital)
                cout << "Country " << country << " hasn't changed its capital" << endl;
            else
                cout << "Country " << country << " has changed its capital from " << capitals[country] << " to " << new_capital << endl;
            capitals[country] = new_capital;
        } else if(opcode == "RENAME") {
            string old_country_name, new_country_name;
            cin >> old_country_name >> new_country_name;
            if( !capitals.count(old_country_name) || capitals.count(new_country_name) || 
            (capitals.count(old_country_name) && (old_country_name == new_country_name))) {
                cout << "Incorrect rename, skip" << endl;
            } else {
                cout << "Country " << old_country_name << " with capital " << capitals[old_country_name] << " has been renamed to " << new_country_name << endl;
                capitals[new_country_name] = capitals[old_country_name];
                capitals.erase(old_country_name);
            }
        } else if (opcode == "ABOUT") {
            string country;
            cin >> country;
            if (!capitals.count(country)) {
                cout << "Country " << country << " doesn't exist" << endl;
            } else {
                cout << "Country " <<  country << " has capital " << capitals[country] << endl;
            }
        } else if (opcode == "DUMP") {
            if (capitals.size() == 0) {
                cout << "There are no countries in the world" << endl;
            } else {
                for (const auto &c : capitals) {
                    cout << c.first << "/" << c.second << " ";
                }
                cout << endl;
            }
        }
    }

    return 0;
}