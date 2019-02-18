#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
using namespace std;

int main(void) {
    unsigned int num_days;
    cin >> num_days;
    vector<int> temps(num_days);
    // Гарантируется, что N не превышает миллиона (10^6), а значения температуры, 
    // измеряющиеся в миллионных долях градусов по Цельсию, лежат в диапазоне от −10^8 до 10^8.
    // 10^6 * 10^8 = 10^14, can fit into int64_t
    int64_t avg = 0;
    for(auto& temp : temps) {
        cin >> temp;
        // int casted to int64_t, safe
        avg += temp;
    }
    // size_t is unsigned, static_cast needed to explicitly make it signed
    avg /= static_cast<int64_t>(temps.size());
    
    // size_t is used for index
    vector<size_t> days;
    for(size_t i = 0; i < temps.size(); i++) {
        // int casted to int64_t, safe
        if(temps[i] > avg) {
            days.push_back(i);
        }
    }
    cout << days.size() << endl;
    for(auto day : days) {
        cout << day << " ";
    }
    return 0;
}