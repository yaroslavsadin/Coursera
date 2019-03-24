#include <iostream>

using namespace std;

class Animal {
public:
    // ваш код
    Animal(const string& t) : Name(t) {}

    const string Name;
};


class Dog : public Animal {
public:
    // ваш код
    Dog(const string& t) {}

    void Bark() {
        cout << Name << " barks: woof!" << endl;
    }
};

int main(void) {
    Dog d("DOG");
    d.Bark();
    return 0;
}