#include <iostream>
#include <vector>

using namespace std;

int main() {
  vector<int> v{1, 2, 3};
  int& ref = v[0];
  v.push_back(4);
  cout << ref << endl;
  return 0;
}