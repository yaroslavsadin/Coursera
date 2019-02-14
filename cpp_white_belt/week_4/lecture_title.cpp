#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

struct Specialization{
    string specialization;
    explicit Specialization(string s) : specialization(s) {}
};

struct Course{
    string course;
    explicit Course(string s) : course(s) {}
};

struct Week{
    string week;
    explicit Week(string s) : week(s) {}
};

class LectureTitle {
public:
// TODO: const ref!!!
    LectureTitle(const Specialization& s, const Course& c, const Week& w) : 
    specialization(s.specialization), course(c.course), week(w.week) {}
  string specialization;
  string course;
  string week;
};

int main(void) {
#if 1 
    LectureTitle title(
    Specialization("C++"),
    Course("White belt"),
    Week("4th")
    );

    if (title.specialization != "C++") {
        return -1;
    }

    cout << title.course << endl;
#else
    LectureTitle title("C++", "White belt", "4th");

    LectureTitle title(string("C++"), string("White belt"), string("4th"));

    LectureTitle title = {"C++", "White belt", "4th"};

    LectureTitle title = {{"C++"}, {"White belt"}, {"4th"}};

    LectureTitle title(
        Course("White belt"),
        Specialization("C++"),
        Week("4th")
    );

    LectureTitle title(
        Specialization("C++"),
        Week("4th"),
        Course("White belt")
    );
#endif
    return 0;
}