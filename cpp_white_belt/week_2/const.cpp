#include <iostream>
using namespace std;

void concat_print(string a, string b) {
    const string concat = a + b;
    cout << concat;
}

int main(void) {
    concat_print("Yaroslav","Sadin");
    return 0;
}