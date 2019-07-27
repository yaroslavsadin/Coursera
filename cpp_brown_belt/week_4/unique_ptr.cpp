#include "test_runner.h"

#include <cstddef>  // нужно для nullptr_t

using namespace std;

#define LOG_FUNCTION() cerr << __FUNCTION__ << endl

// Реализуйте шаблон класса UniquePtr
template <typename T>
class UniquePtr {
private:
  T* ptr_ = nullptr;
  void DeletePtr() {
    if (ptr_ != nullptr) {
      delete ptr_;
    }
  }
public:
  UniquePtr() {}
  UniquePtr(T * ptr) : ptr_(ptr) {}
  UniquePtr(const UniquePtr&) = delete;
  UniquePtr(UniquePtr&& other) {
    if(this != &other) {
      ptr_ = other.ptr_;
      other.ptr_ = nullptr;
    }
  }
  UniquePtr& operator = (const UniquePtr&) = delete;
  UniquePtr& operator = (nullptr_t) {
    DeletePtr();
    ptr_ = nullptr;
  }
  UniquePtr& operator = (UniquePtr&& other) {
    if(this != &other) {  
      ptr_ = other.ptr_;
      other.ptr_ = nullptr;
    }
  }
  ~UniquePtr() {
    DeletePtr();
  }

  T& operator * () const {
    return *ptr_;
  }

  T * operator -> () const {
    return ptr_;
  }

  T * Release() {
    T* ptr = ptr_;
    ptr_ = nullptr;
    return ptr;
  }

  void Reset(T * ptr) {
    if(ptr != ptr_) {
      DeletePtr();
      ptr_ = ptr;
    }
  }

  void Swap(UniquePtr& other) {
    T* ptr = other.ptr_;
    other.ptr_ = this->ptr_;
    this->ptr_ = ptr;
  }

  T * Get() const {
    return ptr_;
  }
};


struct Item {
  static int counter;
  int value;
  Item(int v = 0): value(v) {
    ++counter;
  }
  Item(const Item& other): value(other.value) {
    ++counter;
  }
  ~Item() {
    --counter;
  }
};

int Item::counter = 0;


void TestLifetime() {
  Item::counter = 0;
  {
    UniquePtr<Item> ptr(new Item);
    ASSERT_EQUAL(Item::counter, 1);

    ptr.Reset(new Item);
    ASSERT_EQUAL(Item::counter, 1);
  }
  ASSERT_EQUAL(Item::counter, 0);

  {
    UniquePtr<Item> ptr(new Item);
    ASSERT_EQUAL(Item::counter, 1);

    auto rawPtr = ptr.Release();
    ASSERT_EQUAL(Item::counter, 1);

    delete rawPtr;
    ASSERT_EQUAL(Item::counter, 0);
  }
  ASSERT_EQUAL(Item::counter, 0);
}

void TestGetters() {
  UniquePtr<Item> ptr(new Item(42));
  ASSERT_EQUAL(ptr.Get()->value, 42);
  ASSERT_EQUAL((*ptr).value, 42);
  ASSERT_EQUAL(ptr->value, 42);
}

void TestSelf() {
  UniquePtr<int> ptr(new int(42));
  ptr = move(ptr);
  UniquePtr<int> ptr2(move(ptr));
  ASSERT_EQUAL(ptr.Get(),static_cast<int*>(nullptr));
  ASSERT_EQUAL(*ptr2.Get(),42);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestLifetime);
  RUN_TEST(tr, TestGetters);
  RUN_TEST(tr, TestSelf);
}
