#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool IsPalindrom(string n) {
    if (n.size() == 0) return true;
    for(int i=0,j=n.size()-1; i <= j; i++, j--) {
        char a = n[i];
        char b = n[j];
        if (a != b) return false;
    }
    return true;
}

vector<string> PalindromFilter(vector<string> in, int minLength) {
    vector<string> out;
    for(auto s : in) {
        if (IsPalindrom(s) && s.size() >= minLength)
            out.push_back(s);
    }
    return out;
}

int main(void) {
    vector<string> in = {"bool","poop","madam","bratishka","baoab","lool","boobs"};
    vector<string> out = PalindromFilter(in,6);
    for (auto s : out)
        cout << s << endl;
    return 0;
}
