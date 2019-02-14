#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

class Incognizable {
public:
    Incognizable(){}
    Incognizable(int a) : a(a) {}
    Incognizable(int a, int b) : a(a), b(b) {}
private:
    int a,b;
};

int main(void) {
  Incognizable a;
  Incognizable b = {};
  Incognizable c = {0};
  Incognizable d = {0, 1};
  return 0;
}