#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

using namespace std;

string ConcatenateStringViews(
    string_view lhs, string_view rhs) {
  string s;
  s.reserve(lhs.size() + rhs.size());
  return (s += lhs) += rhs;
}   

int main() {
    string a = "aaaaaaaaaaa";
    string b = "Bbbbbbbbbb";
    string_view sa = a;
    string_view sb = b;
    cout << ConcatenateStringViews(sa,sb) << "\n";
    return 0;
}