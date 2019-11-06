#include <iostream>
#include <variant>
#include <vector>
#include <map>
#include <functional>

template<typename Prev>
class A;
template<typename Prev>
class B;
template<typename Prev>
class C;

class TopLevelGuard {};

static TopLevelGuard guard;

template<typename Prev>
class A {
public:
    using ThisType = A<Prev>;
    A(Prev* prev = &guard) : p(prev) {
        std::cout << "Constructing A" << std::endl;
    }

    B<ThisType> SpawnB() {
        std::cout << __FUNCTION__ << std::endl;
        return B(this);
    }

    C<ThisType> SpawnC() {
        std::cout << __FUNCTION__ << std::endl;
        return C(this);
    }

    Prev& GoBack() {
        std::cout << __FUNCTION__ << std::endl;
        return *p;
    }

    ~A() {
        std::cout << "Destructing A at " << this << std::endl;
    }
private:
    Prev* p;
};

template<typename Prev>
class B {
public:
    using ThisType = B<Prev>;
    B(Prev* prev = &guard) : p(prev) {
        std::cout << "Constructing B" << std::endl;
    }

    A<ThisType> SpawnA() {
        std::cout << __FUNCTION__ << std::endl;
        return A(this);
    }
    
    C<ThisType> SpawnC() {
        std::cout << __FUNCTION__ << std::endl;
        return C(this);
    }

    Prev& GoBack() {
        std::cout << __FUNCTION__ << std::endl;
        return *p;
    }


    ~B() {
        std::cout << "Destructing B at " << this << std::endl;
    }
private:
    Prev* p = nullptr;
};

template<typename Prev>
class C {
public:
    using ThisType = C<Prev>;
    C(Prev* prev = &guard) : p(prev) {
        std::cout << "Constructing C" << std::endl;
    }

    A<ThisType> SpawnA() {
        std::cout << __FUNCTION__ << std::endl;
        return A(this);
    }
    
    B<ThisType> SpawnB() {
        std::cout << __FUNCTION__ << std::endl;
        return B(this);
    }

    Prev& GoBack() {
        std::cout << __FUNCTION__ << std::endl;
        return *p;
    }

    ~C() {
        std::cout << "Destructing C at " << this << std::endl;
    }
private:
    Prev* p;
};

int main(void) {
    {
        auto top = A<TopLevelGuard>();
        top.SpawnB().SpawnC().GoBack().SpawnC().GoBack().SpawnA().SpawnB().GoBack().GoBack().SpawnA();
    }
    std::variant<std::vector<int>,std::vector<int>*> test;
    test = std::vector<int>();
    std::vector<int>* vec_ref = &std::get<std::vector<int>>(test);
    vec_ref->push_back(1);
    vec_ref->push_back(2);
    vec_ref->push_back(3);
    vec_ref->push_back(4);
    for(int i: std::get<std::vector<int>>(test)) {
        std::cout << i << '\n';
    }
    return 0;
}