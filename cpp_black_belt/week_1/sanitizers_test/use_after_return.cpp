#include <iostream>
#include <tuple>

using namespace std;

auto GetTuple() {
  int x = 8, y = 21;
  return tie(x, y);
}

int main() {
  auto [x, y] = GetTuple();
  cout << x << ' ' << y << endl;
  return 0;
}