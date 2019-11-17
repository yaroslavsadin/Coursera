#include <iostream>
using namespace std;

struct A {
  long double ld;   // 8
  // padding 8
  __int128_t li;    // 16
};

int main() {
    cout << sizeof(A) << '\n' << alignof(A) << endl;
    return 0;
}