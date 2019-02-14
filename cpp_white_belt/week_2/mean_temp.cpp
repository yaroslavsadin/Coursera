#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main(void) {
    int num_days;
    cin >> num_days;
    vector<int> temps(num_days);
    int sum = 0;
    for (auto& temp : temps) {
        cin >> temp;
        sum += temp;
    }

    if (num_days)    
        sum /= num_days;
    
    vector<int> temps_out;

    for (int i = 0; i < temps.size(); i++) {
        if (temps[i] > sum) {
            temps_out.push_back(i);
        }
    }

    cout << temps_out.size() << endl;

    for (auto temp : temps_out) {
        cout << temp << " ";
    }
    
    return 0;
}