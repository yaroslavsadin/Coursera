#include <iostream>
#include <string>
#include <vector>
using namespace std;

class B {
public:
    B() {
        cerr << "B constructor" << endl;
    }
    ~B() {
        cerr << "B destructor" << endl;
    }
};

class C {
public:
    C() : b1(B()) {
        b = B();
        cerr << "C constructor" << endl;
        throw exception();
    }
    ~C() {
        cerr << "C destructor" << endl;
    }
private:
    B b;
    B b1;
};

class A {
public:
    A() : b(B()), c(C()) {
        cerr << "A constructor" << endl;
    }
    ~A() {
        cerr << "A destructor" << endl;
    }
private:
    B b;
    C c;
};

int main(void) {
    try {   
        A a;
    } catch(exception& e) {
        cerr << e.what() << endl;
    }
    return 0;
}