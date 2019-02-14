#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

class Student {
public:
    string name;
    string surname;
    int day;
    int month;
    int year;
};

string GetStudentInfo(const vector<Student>& students, const string& request, int num) {
        if(num - 1 > students.size()) {
            return "bad request";
        }
        const Student& student = students[num-1];
        if(request == "name") {
            return (student.name + " " + student.surname);
        } else if(request == "date") {
            return(to_string(student.day) + "." + to_string(student.month) + "." + to_string(student.year));
        } else {
            return "bad request";
        }
    }

int main(void) {
    int num_students;
    cin >> num_students;
    vector<Student> students(num_students);
    for(auto& student : students) {
        cin >> student.name >> student.surname >>
        student.day >> student.month >> student.year;
    }
    int num_requests;
    cin >> num_requests;
    for(;num_requests > 0; num_requests--) {
        string request;
        int num;
        cin >> request >> num;
        cout << GetStudentInfo(students, request, num) << endl;
    }
    return 0;
}