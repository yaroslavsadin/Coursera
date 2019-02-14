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
    ofstream output{"output.txt"};
    string s;
    if(input) {    
        while(getline(input,s)) {
            output << s << endl;
        }
    }
    return 0;
}