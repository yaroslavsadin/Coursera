#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Person {
public:
    Person(string name, string occupation) : name_(name),occupation_(occupation) {}
    string GetName() const {
        return name_;
    }
    virtual void Walk(string destination) {
        cout << occupation_ << ": " << name_ << " walks to: " << destination << endl;
    }
protected:
    string name_,occupation_;
};

class Student : public Person {
public:

    Student(string name, string favouriteSong) : Person(name,"Student"), FavouriteSong(favouriteSong) {}

    void Learn() {
        cout << "Student: " << GetName() << " learns" << endl;
    }

    void SingSong() {
        cout << "Student: " << GetName() << " sings a song: " << FavouriteSong << endl;
    }

    void Walk(string destination) {
        cout << "Student: " << GetName() << " walks to: " << destination << endl;
        SingSong();
    }

private:
    string FavouriteSong;
};


class Teacher : public Person {
public:

    Teacher(string name, string subject) : Person(name,"Teacher"),Subject(subject) {}

    void Teach() {
        cout << "Teacher: " << GetName() << " teaches: " << Subject << endl;
    }

private:
    string Subject;
};


class Policeman : public Person {
public:
    Policeman(string name) : Person(name,"Policeman"),Name(name) {}

    void Check(Teacher t) {
        cout << "Policeman: " << Name << " checks Teacher. Teacher's name is: " << t.GetName() << endl;
    }

    void Check(Student s) {
        cout << "Policeman: " << Name << " checks Student. Student's name is: " << s.GetName() << endl;
    }

    void Check(Policeman p) {
        cout << "Policeman: " << Name << " checks Policeman. Policeman's name is: " << p.GetName() << endl;
    }

public:
    string Name;
};


void VisitPlaces(Teacher t, vector<string> places) {
    for (auto p : places) {
        t.Walk(p);
    }
}

void VisitPlaces(Student s, vector<string> places) {
    for (auto p : places) {
        s.Walk(p);
    }
}

void VisitPlaces(Policeman pol, vector<string> places) {
    for (auto p : places) {
        pol.Walk(p);
    }
}


int main() {
    Teacher t("Jim", "Math");
    Student s("Ann", "We will rock you");
    Policeman p("Bob");

    VisitPlaces(t, {"Moscow", "London"});
    p.Check(s);
    VisitPlaces(s, {"Moscow", "London"});
    return 0;
}
