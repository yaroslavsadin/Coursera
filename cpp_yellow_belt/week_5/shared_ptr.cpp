#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
#include <memory>
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

class Cat : public Animal {
public:
    Cat() : Animal("cat") {}
    void Voice(void) const override {
        cout << "Meow" << endl;
    }
};

class Dog : public Animal {
public:
    Dog() : Animal("Dog") {}
    void Voice(void) const override {
        cout << "Woof" << endl;
    }
};

void MakeVoice(const Animal& a) {
    a.Voice();
}

int main(void) {
    vector<shared_ptr<Animal>> animals = {
        make_shared<Pig>(),
        make_shared<Cat>(),
        make_shared<Dog>(),
    };

    for(auto a : animals) {
        MakeVoice(*a);
    }
    return 0;
}