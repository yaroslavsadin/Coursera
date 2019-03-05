#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

template <typename T>
void PrintReversed(T begin_range, T end_range) {
    auto it = end_range;
    while (it != begin_range) {
        it--;
        cout << *it << " ";
    }
}

void PrintVectorPart(const vector<int>& numbers) {
    auto it = find_if(numbers.begin(), numbers.end(),[](int a) {return (a < 0);});
    if (it == numbers.end()) {
        PrintReversed(numbers.begin(), numbers.end());
    } else {
        PrintReversed(numbers.begin(), it);
    }
}

int main(void) {
    PrintVectorPart({6, 1, 8, -5, 4});
    cout << endl;
    PrintVectorPart({-6, 1, 8, -5, 4});  // ничего не выведется
    cout << endl;
    PrintVectorPart({6, 1, 8, 5, 4});
    cout << endl;
    return 0;
}