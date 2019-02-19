#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
using namespace std;

template <class KeyT,class ValueT> ValueT& GetRefStrict(map<KeyT,ValueT>& m, KeyT k) {
    if(m.count(k)) {
        return m.at(k);
    } else {
        throw(runtime_error("No such key"));
    }
}

int main(void) {
    map<int, string> m = {{0, "value"}};
    string& item = GetRefStrict(m, 0);
    item = "newvalue";
    cout << m[0] << endl; // выведет newvalue
    return 0;
}