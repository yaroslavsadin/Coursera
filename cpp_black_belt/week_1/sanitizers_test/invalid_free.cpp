#include <iostream>
#include <memory>

using namespace std;

int main() {
  const int x = 7;
  unique_ptr<const int> q(&x);
  cout << x << ' ' << *q << endl;
  return 0;
}