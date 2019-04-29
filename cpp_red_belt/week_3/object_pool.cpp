#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:
  T* Allocate() {
    T* p = TryAllocate();
    if(!p) {
      p = new T;
      alloced.insert(p);
    }
    return p;
  }
  T* TryAllocate() {
    T* p;
    if(dealloced.size()) {
      p = dealloced.front();
      alloced.insert(p);
      dealloced.pop();
    } else {
      p = nullptr;
    }
    return p;
  }

  void Deallocate(T* object) {
    auto it = alloced.find(object);
    if(it != alloced.end()) {
      dealloced.push(object);
      alloced.erase(it);
    } else {
      throw invalid_argument("Pointer doesn't exist");
    }
  }

  ~ObjectPool() {
    for(auto& ptr : alloced) {
      delete ptr;
    }
    while(dealloced.size()) {
      delete dealloced.front();
      dealloced.pop();
    }
  }

private:
  // Добавьте сюда поля
  set<T*> alloced;
  queue<T*> dealloced;
};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}
