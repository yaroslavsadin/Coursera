#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

set<string> BuildMapValuesSet(const map<int,string>& m) {
    set <string> s;
    for(auto &i : m) {
        s.insert(i.second);
    }
    return s;
}

#if 0
int main(void) {
    set<string> values = BuildMapValuesSet({
    {1, "odd"},
    {2, "even"},
    {3, "odd"},
    {4, "even"},
    {5, "odd"}
});

for (const string& value : values) {
  cout << value << endl;
}
}
#endif