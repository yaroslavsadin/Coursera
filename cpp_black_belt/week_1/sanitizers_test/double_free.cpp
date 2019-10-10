#include <iostream>
#include <memory>

using namespace std;

int main() {
  auto p = make_unique<int>(6);
  unique_ptr<int> q(p.get());
  cout << *p << ' ' << *q << endl;
  return 0;
}