#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

class Lecture {
public:
    string name = "C++";
    int duration = 2;
};

int main(void) {
    Lecture lecture = {.name = "lol"};
    cout << lecture.duration << " " << lecture.name << endl;
    return 0;
}