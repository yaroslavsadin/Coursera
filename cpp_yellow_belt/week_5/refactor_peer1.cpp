//
//  refactingCod.cpp
//  Refacting cod
//
//  Created by Sergei Ziuzev on 3/24/19.
//  Copyright В© 2019 Sergei Ziuzev. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Person {
public:
    Person(const string& type, const string& name): _type(type), _name(name) {}
    
    virtual void Walk(const string& destination) {
        cout << PritnTypeName() << " walks to: " << destination << endl;
    }
    
    virtual string GetName() const {
        return _name;
    }
    virtual string GetType() const {
        return _type;
    }
    
    virtual string PritnTypeName() {
        return GetType() + ": " + GetName();
    }
    
private:
    const string _type;
    const string _name;
};

class Student: public Person {
public:
    
    Student(const string& name, const string& favouriteSong): Person("Student", name), _favouriteSong(favouriteSong) {}
    
    void Learn() {
        cout << PritnTypeName() << " learns" << endl;
    }
    
    void Walk(const string& destination) {
        cout << PritnTypeName() << " walks to: " << destination << endl;
        SingSong();
    }
    
    void SingSong() {
        cout << PritnTypeName() << " sings a song: " << _favouriteSong << endl;
    }
    
private:
    const string _favouriteSong;
};

class Teacher: public Person {
public:
    
    Teacher(const string& name, const string& subject): Person("Teacher", name), _subject(subject) {}

    
    void Teach() {
        cout << PritnTypeName() << " teaches: " << _subject << endl;
    }
    
private:
    const string _subject;
};

class Policeman: public Person {
public:
    Policeman(const string& name): Person("Policeman", name) {}
    
    void Check(const Person& person) {
        cout << PritnTypeName()  << " checks " << person.Person::GetType() << ". " << person.Person::GetType() << "'s name is: " << person.Person::GetName() << endl;
    }

};

void VisitPlaces(Person& person, vector<string> places) {
    for (auto place: places) {
        person.Walk(place);
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