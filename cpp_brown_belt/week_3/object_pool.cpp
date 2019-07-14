#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <unordered_set>
using namespace std;

template <class T>
class ObjectPool {
public:
  T* Allocate();
  T* TryAllocate();

  void Deallocate(T* object);

  ~ObjectPool();

private:
  // Добавьте сюда поля
  unordered_set<T*> active;
  queue<T*> dealocced;
};

template <class T>
ObjectPool<T>::~ObjectPool() {
  while(!dealocced.empty()) {
    T* ptr = dealocced.front();
    dealocced.pop();
    delete ptr;
  }
  for(auto ptr : active) {
    delete ptr;
  }
}

template <class T>
void ObjectPool<T>::Deallocate(T* object) {
  if(active.count(object)) {
    dealocced.push(object);
    active.erase(object);
  } else {
    throw invalid_argument("No such object");
  }
}

template <class T>
T* ObjectPool<T>::TryAllocate() {
  if(!dealocced.empty()) {
    auto [it,success] = active.insert(dealocced.front());
    dealocced.pop();
    return *it;
  } else {
    return nullptr;
  }
}

template <class T>
T* ObjectPool<T>::Allocate() {
  T* obj_ptr = TryAllocate();
  if(obj_ptr == nullptr) {
    obj_ptr = new T();
    auto [it,success] = active.insert(obj_ptr);
    return *it;
  }
  return obj_ptr;
}

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
