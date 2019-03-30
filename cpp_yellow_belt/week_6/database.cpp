#include "database.h"
#include <iterator>
#include <algorithm>
#include "test_framework.h"

void Database::Add(Date date, string event) {
    db[date].insert(event);
}

void Database::Print(const ostream& os) const {
    for(const auto& date : db) {
        for(const auto& event : date.second) {
        cout << date.first << " " << event << endl;
        }
    }
}

string Database::Last(Date date) {
    auto it = lower_bound(db.begin(),db.end(),date);
    if (it == db.end()) {
        it = prev(it);
    }
    return *prev(it->second.end());
}

void TestDatabase(void) {
    Database db;
    Date d(1990,06,21);
    string expected = "Birthday";
    db.Add(d,expected);
    string result = db.Last(d);
    AssertEqual(expected,result,"Add falied",__LINE__,__FILE__);
    db.Print(cout);
}

int main(void) {
    TestRunner tr;
    tr.RunTest(TestDatabase,"TestDatabase");
    return 0;
}
