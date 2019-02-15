#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

class Lecture {
public:
    string name = "kek";
    int duration;
};

int main(void) {
    Lecture lecture = {.name = "lol", .duration = 4};
    cout << lecture.duration << " " << lecture.name << endl;
    return 0;
}