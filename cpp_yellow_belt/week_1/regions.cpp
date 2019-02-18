#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
#include <tuple>
#include <utility>
using namespace std;

#if 1
enum class Lang {
  DE, FR, IT
};

struct Region {
  string std_name;
  string parent_std_name;
  map<Lang, string> names;
  int64_t population;
};
#endif

bool operator==(const Region& lhs,const Region& rhs) {
    tuple tlhs(lhs.std_name,lhs.parent_std_name,lhs.names,lhs.population);
    tuple trhs(rhs.std_name,rhs.parent_std_name,rhs.names,rhs.population);
    bool res = (tlhs == trhs);
    return res;
}

bool operator<(const Region& lhs,const Region& rhs) {
    tuple tlhs(lhs.std_name,lhs.parent_std_name,lhs.names,lhs.population);
    tuple trhs(rhs.std_name,rhs.parent_std_name,rhs.names,rhs.population);
    bool res = (tlhs < trhs);
    return res;
}
/* Better :
int FindMaxRepetitionCount(const vector<Region>& regions) {
  int result = 0;
  map<Region, int> repetion_count;
  for (const Region& region : regions) {
    result = max(result, ++repetion_count[region]);
  }
  return result;
}
*/

int FindMaxRepetitionCount(const vector<Region>& regions) {
    if(!regions.size()) 
        return 0;
    
    map<Region,int> region_counts;
    for(const auto& r :regions) {
        (region_counts.count(r)) ? region_counts[r]++ :
                                   region_counts[r] = 1;
    }

    int max_rep = 1;
    for(const auto& [r,c] : region_counts) {
        if(c > max_rep) max_rep = c;
    }

    return max_rep;
}

int main() {
  cout << FindMaxRepetitionCount({
      {
          "Moscow",
          "Russia",
          {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
          89
      }, {
          "Russia",
          "Eurasia",
          {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
          89
      }, {
          "Moscow",
          "Russia",
          {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
          89
      }, {
          "Moscow",
          "Russia",
          {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
          89
      }, {
          "Russia",
          "Eurasia",
          {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
          89
      },
      {
          "Russia",
          "Eurasia",
          {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
          89
      },
  }) << endl;
  
  cout << FindMaxRepetitionCount({
      {
          "Moscow",
          "Russia",
          {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
          89
      }, {
          "Russia",
          "Eurasia",
          {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
          89
      }, {
          "Moscow",
          "Russia",
          {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou deux"}, {Lang::IT, "Mosca"}},
          89
      }, {
          "Moscow",
          "Toulouse",
          {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
          89
      }, {
          "Moscow",
          "Russia",
          {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
          31
      },
  }) << endl;
  
  return 0;
}
