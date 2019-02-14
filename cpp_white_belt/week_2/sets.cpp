#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

int main(void) {
    set<string> famous_persons;
    famous_persons.insert("Stroustrup");
    famous_persons.insert("Ritchie");
    cout << famous_persons.size() << endl;
    for(auto x : famous_persons) {
        cout << x << endl;
    }
    famous_persons = {};
    cout << famous_persons.size() << endl;
    return 0;
}