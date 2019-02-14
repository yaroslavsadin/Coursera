#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

int main(void) {
    ifstream input{"input.txt"};
    string s;
    if(input) {    
        while(getline(input,s)) {
            cout << s << endl;
        }
    }
    return 0;
}