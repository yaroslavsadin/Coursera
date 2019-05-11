#include <iostream>
#include <list>
using namespace std;

template <typename T, typename Iterator>
void SwapListElements(list<T>& l, Iterator i1, Iterator i2) {
    auto temp = next(i2);
    l.splice(i1,l,i2);
    l.splice(temp,l,i1);
}

int main(void) {
    list<int> l = {1,2,3,4,5,6};
    auto it1 = l.begin();
    auto it2 = --l.end();
    cout << *it1 << " " << *it2 << "\n";
    SwapListElements(l,it1,it2);
    for(const auto& item : l) {
        cout << item << endl;
    }
    return 0;
}