#include <iostream>
#include <vector>
#include <string>
using namespace std;

void ElemSwap(vector<int>& v,int i,int j) {
    int a = v[i];
    v[i] = v[j];
    v[j] = a;
}

void Reverse(vector<int>& v) {
    if (!v.size() || v.size() == 1) return;
    for(int i = v.size()-1; i >= v.size()/2; i--) {
        ElemSwap(v,i,v.size()-1-i);
    }
}

int main(void) {
    vector<int> source = {1, 2};
    Reverse(source);
    for(auto i : source) {
        cout << i << " ";
    }
    return 0;
}

