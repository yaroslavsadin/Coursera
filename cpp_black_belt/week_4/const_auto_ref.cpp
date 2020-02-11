#include <vector>
#include <iostream>

using namespace std;

template<typename T>
auto& GetRefToElem(vector<T>& v, size_t  idx) {
    return v[idx];
}

template<typename T>
void Print(const vector<T>& v) {
    for(const T& elem : v) {
        cout << elem << ' ';
    }
    cout << endl;
}

int main() {
    vector<int> v = {1,2,3,4};
    Print(v);
    GetRefToElem(v,1) = 666;
    Print(v);
    vector<bool> v1 = {true,false,false};
    Print(v1);
    GetRefToElem(v1,1) = true;
    Print(v1);
    return 0;
}