#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
using namespace std;

class Animal {
public:
    Animal(const string& name) : name_(name) {cout << "Make " << name << endl;}
    virtual void Voice(void) const {
        cout << name_ << " has no voice" << endl;
    }

private:
    string name_;
};

class Pig : public Animal {
public:
    Pig() : Animal("pig") {}
    void Voice(void) const override {
        cout << "squeal" << endl;
    }
};

void MakeVoice(const Animal& a) {
    a.Voice();
}

int main(void) {
    vector<Animal> animals;
    Pig p;
    animals.push_back(p);
    MakeVoice(p);
    MakeVoice(animals[0]);
    return 0;
}