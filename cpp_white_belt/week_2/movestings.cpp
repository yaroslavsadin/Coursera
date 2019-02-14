#include <iostream>
#include <vector>
#include <string>
using namespace std;

void MoveStrings(vector<string>& source, vector<string>& destination) {
    for(auto s : source) {
        destination.push_back(s);
    }
    source.clear();
}

int main(void) {
    vector<string> source = {"a", "b", "c"};
    vector<string> destination = {"z"};
    MoveStrings(source, destination);
    for(auto s : source) {
        cout << s << " ";
    }
    cout << endl;
    for(auto s : destination) {
        cout << s << " ";
    }
    cout << endl;
    return 0;
}

