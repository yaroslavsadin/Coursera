#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool Contains(vector<string> words, string word) {
    for (auto w : words) {
        if(w == word) {
            return true;
        }
    }
    return false;
}

int main(void) {
    cout << Contains({"air", "water", "fire"}, "water");
    return 0;
}
