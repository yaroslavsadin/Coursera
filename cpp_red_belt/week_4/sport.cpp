#include <iostream>
#include <fstream>
#include <set>
#include <list>
#include <algorithm>
using namespace std;

class AtheltManager {
public:
    void Add(int number, int before_this) {
        auto found = athlets_.find({.number = before_this});
        if(found != athlets_.end()) {
            athlets_.insert({
                .number = number, 
                .it = athlets.insert(found->it,number)
            });
        } else {
            athlets.push_back(number);
            athlets_.insert({
                .number = number, 
                .it = prev(athlets.end())
            });
        }
    }
    void Print(void) const {
        for(const auto& item : athlets) {
            cout << item << " ";
        }
    }
private:
    struct Athlet {
    int number;
    list<int>::iterator it;
    bool operator<(const Athlet& other) const {
        return number < other.number;
    }
};

    list<int> athlets;
    set<Athlet> athlets_;
};

int main(void) {
    cin.tie ( nullptr );
    ios_base::sync_with_stdio ( false ) ;

    AtheltManager am;
    int count;
    cin >> count;
    while(count--) {
        int n1, n2;
        cin >> n1 >> n2;
        am.Add(n1,n2);
    }
    am.Print();
    
    return 0;
}