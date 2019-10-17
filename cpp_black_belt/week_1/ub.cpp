#include <iostream>
#include <limits>
#include "test_runner.h"

bool CheckSumOverflow(std::int64_t a, std::int64_t b) {
    if( a > 0 && b > 0 ) {
        // Use std::numeric_limits<T>::max()
        if (a > INT64_MAX - b) {
            return true;
        }
    } else if( a < 0 && b < 0 ) {
        if (a < INT64_MIN - b) {
            return true;
        }
    }
    return false;
}

void TestOverflow() {
    {
        std::int64_t a = 0;
        std::int64_t b = 0;
        ASSERT(!CheckSumOverflow(a,b));
    }
    {
        std::int64_t a = 0;
        std::int64_t b = INT64_MIN;
        ASSERT(!CheckSumOverflow(a,b));
    }
    {
        std::int64_t a = INT64_MIN;
        std::int64_t b = INT64_MIN;
        ASSERT(CheckSumOverflow(a,b));
    }
    {
        std::int64_t a = INT64_MAX;
        std::int64_t b = INT64_MIN;
        ASSERT(!CheckSumOverflow(a,b));
    }
    {
        std::int64_t a = INT64_MAX;
        std::int64_t b = INT64_MAX;
        ASSERT(CheckSumOverflow(a,b));
    }
    {
        std::int64_t a = -1;
        std::int64_t b = INT64_MIN;
        ASSERT(CheckSumOverflow(a,b));
    }
    {
        std::int64_t a = 1;
        std::int64_t b = INT64_MAX;
        ASSERT(CheckSumOverflow(a,b));
    }
    {
        std::int64_t a = -1000;
        std::int64_t b = INT64_MAX;
        ASSERT(!CheckSumOverflow(a,b));
    }
    {
        std::int64_t a = -1000;
        std::int64_t b = INT64_MIN;
        ASSERT(CheckSumOverflow(a,b));
    }
    {
        std::int64_t a = INT64_MIN;
        std::int64_t b = 1;
        ASSERT(!CheckSumOverflow(a,b));
    }
    {
        std::int64_t a = INT64_MAX - 1;
        std::int64_t b = 1;
        ASSERT(!CheckSumOverflow(a,b));
    }
    {
        std::int64_t a = INT64_MIN;
        std::int64_t b = -1;
        ASSERT(CheckSumOverflow(a,b));
    }
    {
        std::int64_t a = INT64_MAX;
        std::int64_t b = -1;
        ASSERT(!CheckSumOverflow(a,b));
    }
    {
        std::int64_t a = INT64_MIN / 2;
        std::int64_t b = INT64_MAX / 2;
        ASSERT(!CheckSumOverflow(a,b));
    }
}

class A {
public:
    A() = default;
    virtual ~A() {
        cerr << __FUNCTION__ << endl;
    }
private:
    string some = "";
};

class B : public A {
public:
    B() = default;
    ~B() {
        cerr << __FUNCTION__ << endl;
    }
private:
    string some = "";
};

int main(void) {
    A* base = new B();
    delete base;
    TestRunner tr;
    RUN_TEST(tr,TestOverflow);
    std::int64_t a, b;
    std::cin >> a >> b;

    if(CheckSumOverflow(a,b)) {
        std::cout << "Overflow!";
    } else {
        std::cout << a + b;
    }
    return 0;
}