#pragma once
#include "date.h"
#include <vector>
#include <map>
#include <deque>

using namespace std;

using Entry = pair<Date,string>;

template<typename T1,typename T2>
ostream& operator<<(ostream& os, pair<T1,T2> p) {
    return os << p.first << " " << p.second;
}

class Database {
public:
    Database() {}
    void Add(Date date, const string& event);
    void Print(ostream& os) const;
    template <typename PredT> 
    int RemoveIf (PredT predicate){
        int res { 0 };
        for(auto& i : db) {
            auto it = i.second.begin();
            while(it != i.second.end()) {
                if (predicate(i.first,*it)) {
                    it = i.second.erase(it);
                    res++;
                } else {
                    it = next(it);
                }
            }
            if (i.second.empty()) {
                db.erase(i.first);
            }
        }
        return res;
    }
    template <typename PredT> 
    vector<Entry> FindIf(PredT predicate) const {
        vector<Entry> res;
        for(auto& i : db) {
            auto it = i.second.begin();
            while(it != i.second.end()) {
                if (predicate(i.first,*it)) {
                    res.push_back(pair<Date,string>(i.first,*it));
                }
                it = next(it);
            }
        }
        return res;
    }
    string Last(Date date);
private:
    map<Date,deque<string>> db;
};

void TestDatabase(void);