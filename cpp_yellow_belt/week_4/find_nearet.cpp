#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
using namespace std;

set<int>::const_iterator FindNearestElement(
    const set<int>& numbers,
    int border) {
        auto p = numbers.equal_range(border);
        if(p.first == p.second) {
            if(p.first == numbers.begin()) {
                return p.first;
            } else if(p.first == numbers.end()) {
                return prev(p.first);
            } else {
                if(*p.first - border >= border - *(prev(p.first))) {
                    return prev(p.first);
                } else {
                    return p.first;
                }
            }
        } else {
            return p.first;
        }
    }
// set<int>::const_iterator —
// тип итераторов для константного множества целых чисел

int main() {
  set<int> numbers = {1, 4, 6};
  cout <<
      *FindNearestElement(numbers, 0) << " " <<
      *FindNearestElement(numbers, 3) << " " <<
      *FindNearestElement(numbers, 5) << " " <<
      *FindNearestElement(numbers, 6) << " " <<
      *FindNearestElement(numbers, 100) << endl;
      
  set<int> empty_set;
  
  cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << endl;
  return 0;
}