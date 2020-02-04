#include "vector.h"

#include "test_runner.h"

using namespace std;

class C {
public:
  inline static int created = 0;
  inline static int assigned = 0;
  inline static int deleted = 0;
  static void Reset() {
    created = assigned = deleted = 0;
  }

  C() {
    ++created;
  }
  C(const C& other) {
    ++created;
  }
  C& operator=(const C& other) {
    ++assigned;
    return *this;
  }
  ~C() {
    ++deleted;
  }
};

void TestInit() {
  {
    C::Reset();
    Vector<C> v(3);
    ASSERT(C::created == 3 && C::assigned == 0 && C::deleted == 0);
  }
  ASSERT(C::deleted == 3);
};


void TestAssign() {
  {
    C::Reset();
    Vector<C> v1(2), v2(3);
    ASSERT(C::created == 5 && C::assigned == 0 && C::deleted == 0);
    v1 = v2;
    ASSERT(C::created == 8 && C::assigned == 0 && C::deleted == 2);
    ASSERT(v1.Size() == 3 && v2.Size() == 3);
  }
  ASSERT(C::deleted == 8);

  {
    C::Reset();
    Vector<C> v1(3), v2(2);
    ASSERT(C::created == 5 && C::assigned == 0 && C::deleted == 0);
    v1 = v2;
    ASSERT(C::created == 5 && C::assigned == 2 && C::deleted == 1);
    ASSERT(v1.Size() == 2 && v2.Size() == 2);
  }
  ASSERT(C::deleted == 5);
}

void TestPushBack() {
  {
    C::Reset();
    Vector<C> v;
    C c;
    v.PushBack(c);
    ASSERT(C::created == 2 && C::assigned == 0 && C::deleted == 0);

    v.PushBack(c);  // reallocation
    ASSERT(C::created == 4 && C::assigned == 0 && C::deleted == 1);
  }
  ASSERT(C::deleted == 4);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestInit);
  RUN_TEST(tr, TestAssign);
  RUN_TEST(tr, TestPushBack);
  {
    vector<int> test;
    test.resize(1);
    cerr << test.capacity() << ' ' << test.size();
    cerr << endl;
    test.resize(2);
    cerr << test.capacity() << ' ' << test.size();
    cerr << endl;
    test.resize(3);
    cerr << test.capacity() << ' ' << test.size();
    cerr << endl;
    test.resize(2);
    cerr << test.capacity() << ' ' << test.size();
    cerr << endl;
    test.resize(1);
    cerr << test.capacity() << ' ' << test.size();
    cerr << endl;
    test.resize(0);
    cerr << test.capacity() << ' ' << test.size();
    cerr << endl;
  }
  {
    Vector<int> test;
    test.Resize(1);
    cerr << test.Capacity() << ' ' << test.Size();
    cerr << endl;
    test.Resize(2);
    cerr << test.Capacity() << ' ' << test.Size();
    cerr << endl;
    test.Resize(3);
    cerr << test.Capacity() << ' ' << test.Size();
    cerr << endl;
    test.Resize(2);
    cerr << test.Capacity() << ' ' << test.Size();
    cerr << endl;
    test.Resize(1);
    cerr << test.Capacity() << ' ' << test.Size();
    cerr << endl;
    test.Resize(0);
    cerr << test.Capacity() << ' ' << test.Size();
    cerr << endl;
  }
  return 0;
}
