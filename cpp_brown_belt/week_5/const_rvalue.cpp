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

B foo() {
    return B();
}

int main(void) {
    cerr << "Calling foo()" << endl;
    const B& b = foo();
    cerr << "After foo()" << endl;
    int y = 5;
    int&& x = y++;
    cerr << x << endl;
    y = 10;
    cerr << ++x << endl;
    cerr << y << endl;
    return 0;
}