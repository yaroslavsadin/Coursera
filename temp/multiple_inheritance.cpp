#include <iostream>

using namespace std;

class A {
public:
    A() {
        cout << __FUNCTION__ << endl;
    }
    virtual int Process() = 0;
};

class B : virtual public A {
public:
    B(int a) {
            cout << __FUNCTION__ << endl;
    }
    int Process() override {
        cout << "B::Process()" << endl;
        return a;
    }
private:
    int a = 1;
};

class C : virtual public A {
public:
    C() {
        cout << __FUNCTION__ << endl;
    }
    int Process() override {
        cout << "C::Process()" << endl;
        return a;
    }
private:
    int a = 2;
};

class D : public B, public C {
public:
    D() : C(), B(C::Process()) {
        cout << __FUNCTION__ << endl;
    }
    int Process() override {
        cout << "D::Process()" << endl;
        return B::Process();
    }
private:
    int a = 3;
};

int main() {
    D d;
    d.Process();
    return 0;
}