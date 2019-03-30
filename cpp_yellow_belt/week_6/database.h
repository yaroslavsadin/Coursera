#pragma once
#include "date.h"
#include <vector>
#include <map>
#include <set>

using namespace std;

using Entry = pair<Date,string>;

class Database {
public:
    Database() {}
    void Add(Date date, string event);
    void Print(const ostream& os) const;
    template <typename PredT> 
    int RemoveIf (PredT predicate){

    }
    template <typename PredT> 
    vector<Entry> FindIf(PredT predicate) const {

    }
    string Last(Date date);
private:
    map<Date,set<string>> db;
};