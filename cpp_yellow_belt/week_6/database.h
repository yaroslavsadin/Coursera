#pragma once
#include "date.h"
#include <vector>
#include <map>
#include <deque>

using namespace std;

using Entry = pair<Date,string>;

class Database {
public:
    Database() {}
    void Add(Date date, const string& event);
    void Print(const ostream& os) const;
    template <typename PredT> 
    int RemoveIf (PredT predicate){

    }
    template <typename PredT> 
    vector<Entry> FindIf(PredT predicate) const {

    }
    string Last(Date date);
private:
    map<Date,deque<string>> db;
};