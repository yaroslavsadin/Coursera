#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

class ReversibleString {
public:
    ReversibleString() {}
    ReversibleString(const string& a) : str(a) {}
    void Reverse(void) {
        for(int i = str.size()-1,j = 0; i >= str.size()/2; i--,j++) {
            swap(str[i],str[j]);
        }
    }
    string ToString(void) const {
        return str;
    }
private:
    string str;
};

int main(void) {
    ReversibleString s("abcde");
    s.Reverse();
    cout << s.ToString() << endl;
    
    s.Reverse();
    const ReversibleString& s_ref = s;
    string tmp = s_ref.ToString();
    cout << tmp << endl;
    
    ReversibleString empty;
    cout << '"' << empty.ToString() << '"' << endl;
    
    return 0;
}