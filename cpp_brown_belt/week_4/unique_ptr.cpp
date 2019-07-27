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
  UniquePtr() : ptr_(nullptr) {}
  UniquePtr(T * ptr) : ptr_(ptr) {}
  UniquePtr(const UniquePtr&) = delete;
  UniquePtr(UniquePtr&& other) : ptr_(other.ptr_) {
    other.ptr_ = nullptr;
  }
  UniquePtr& operator = (const UniquePtr&) = delete;
  UniquePtr& operator = (nullptr_t) {
    Reset(nullptr);
    return *this;
  }
  UniquePtr& operator = (UniquePtr&& other) {
    if(this != &other) {
      ptr_ = other.ptr_;
      other.ptr_ = nullptr;
    }
    return *this;
  }
  ~UniquePtr() {
    DeletePtr();
  }

  T& operator * () const {
    return *Get();
  }

  T * operator -> () const {
    return Get();
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
    if(this != &other) {
      T* ptr = other.ptr_;
      other.ptr_ = this->ptr_;
      this->ptr_ = ptr;
    }
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
  ptr2.Swap(ptr2);
  ptr = move(ptr2);
  int lol_ = *ptr.Get();
  ASSERT_EQUAL(lol_,42);
  ptr2.Reset(ptr.Get());
  ASSERT_EQUAL(ptr2.Get(),ptr.Get());
  ptr2.Reset(ptr2.Get());
  int* lol = ptr.Release();
  ptr2.Reset(lol);
  ASSERT_EQUAL(ptr.Get(),static_cast<int*>(nullptr));
  ASSERT_EQUAL(*ptr2.Get(),42);
}

void TestAll() {
  {  
    // Construct an empty ptr
    UniquePtr<int> ptr;
  }
  {
    // Construct from raw ptr
    UniquePtr<int> ptr(new int(42));
  }
  {
    // Construct from other
    UniquePtr<int> ptr;
    UniquePtr<int> ptr2(move(ptr));
  }
  {
    // Assign to self
    UniquePtr<int> ptr(new int(42));
    ptr = move(ptr);
    ASSERT_EQUAL(*ptr.Get(),42)
  }
  {
    // Swap with self
    UniquePtr<int> ptr(new int(42));
    ptr.Swap(ptr);
    ASSERT_EQUAL(*ptr.Get(),42)
  }
  {
    // Set released to self
    UniquePtr<int> ptr(new int(42));
    ptr.Reset(ptr.Release());
    ptr.Reset(ptr.Get());
    ASSERT_EQUAL(*ptr.Get(),42);
  }
  {
    // Set released to self
    UniquePtr<int> ptr(new int(42));
    UniquePtr<int> ptr2(new int(666));
    ptr.Swap(ptr2);
    ASSERT_EQUAL(*ptr.Get(),666);
    ASSERT_EQUAL(*ptr2.Get(),42);
  }
  {
    // Chaining assignment
    UniquePtr<int> ptr(new int(42));
    UniquePtr<int> ptr2(new int(666));
    UniquePtr<int> ptr3(new int(43));
    ptr = move(ptr2 = move(ptr3));
    ASSERT_EQUAL(*ptr,43);
    ASSERT_EQUAL(ptr2.Get(),static_cast<int*>(nullptr));
    ASSERT_EQUAL(ptr3.Get(),static_cast<int*>(nullptr));
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestLifetime);
  RUN_TEST(tr, TestGetters);
  RUN_TEST(tr, TestSelf);
  RUN_TEST(tr, TestAll);
}
