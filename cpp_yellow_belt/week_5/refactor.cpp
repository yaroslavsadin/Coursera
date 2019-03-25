#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Person {
public:
    Person(const string& name, const string& occupation) : name_(name),occupation_(occupation) {}
    const string& GetName() const {
        return name_;
    }
    const string& GetOccupation() const {
        return occupation_;
    }
    void DoThings(const string& things) {
        cout << occupation_ << ": " << name_ << " " << things << endl;
    }
    virtual void Walk(const string& destination) {
        DoThings("walks to: " + destination);
    }
private:
    const string name_,occupation_;
};

class Student : public Person {
public:

    Student(const string& name, const string& favouriteSong) : Person(name,"Student"), FavouriteSong(favouriteSong) {}

    void Learn() {
        DoThings("learns");
    }

    void SingSong() {
        DoThings("sings a song: "+FavouriteSong);
    }

    void Walk(const string& destination) override {
        DoThings("walks to: " + destination);
        SingSong();
    }

private:
    const string FavouriteSong;
};


class Teacher : public Person {
public:
    Teacher(const string& name, const string& subject) : Person(name,"Teacher"),Subject(subject) {}

    void Teach() {
        DoThings("teaches: "+Subject);
    }

private:
    const string Subject;
};


class Policeman : public Person {
public:
    Policeman(const string& name) : Person(name,"Policeman") {}

    void Check(const Person& t) {
        DoThings("checks " + t.GetOccupation() + ". " + t.GetOccupation() + "'s name is: " + t.GetName());
    }

private:
};


void VisitPlaces(Person& t, vector<string> places) {
    for (auto p : places) {
        t.Walk(p);
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
