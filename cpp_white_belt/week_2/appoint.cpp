#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Magic constants are evil
// Can use % operation instead of if(month_length < 12)
// Define a vector of month lengths is better...

void AdjustDays(int month, vector<vector<string>>& days) {
    switch (month) {
        case 0: 
        case 2:
        case 4:
        case 6:
        case 7:
        case 9:
        case 11: {
            days.resize(31);
            break;
        }
        case 1: {
            for(int i = 28; i < 31; i++) {
                if (days[i].size()) {
                    days[27].insert(end(days[27]), begin(days[i]), end(days[i]));
                }
            }
            days.resize(28);
            break;
        }
        default: {
            if (days[30].size()) {
                days[29].insert(end(days[29]), begin(days[30]), end(days[30]));
            }
            days.resize(30);
        }
    }
}

int main(void) {
    vector<vector<string>> days(31);
    int cur_month = 0;
    
    int num_ops;
    cin >> num_ops;

    for (int i = num_ops; i > 0; i--) {
        string opcode;
        cin >> opcode;

        if(opcode == "ADD") {
            int day;
            string appointment;
            cin >> day >> appointment;
            if (day > days.size())
                day = days.size()-1;
            days[day-1].push_back(appointment);
        } else if(opcode == "NEXT") {
            if(++cur_month > 11)
                cur_month = 0;
            AdjustDays(cur_month, days);
        } else if(opcode == "DUMP") {
            int day;
            cin >> day;
            cout << days[day-1].size() << " ";
            for(auto appointment : days[day-1]) {
                 cout << appointment << " ";
            }
            cout << endl;
        }
    }
    return 0;
}