#include <cstddef>
#include <iostream>
#include <utility>
#include <memory>

template<typename T>
class RawMem {
public:
  RawMem() : data(nullptr), cp(0) {}
  RawMem(size_t n) { // may throw
    data = reinterpret_cast<T*>(new char[sizeof(T) * n]);
    cp = n;
  }
  T* Get() const {
    return data;
  }
  size_t Cp() const {
    return cp;
  }
  T* operator+(size_t i) {
    return data + i;
  }
  T& operator[](size_t i) {
    return *(data + i);
  }
  const T& operator[](size_t i) const {
    return *(data + i);
  }
  ~RawMem() {
    if(data) {  
      delete [] reinterpret_cast<char*>(data);
    }
  }
  void Swap(RawMem& other) noexcept {
    T* data_ = data;
    size_t cp_ = cp;
    data = other.data;
    cp = other.cp;
    other.data = data_;
    other.cp = cp_;
  }
private:
  T* data;
  size_t cp;
};

template <typename T>
class Vector {
public:
  Vector() : sz(0) {}
  Vector(size_t n) : data(n) {
    for(size_t i = 0; i < n; i++) {
      new (data + i) T;
    }
    sz = n;
  }
  Vector(const Vector& other) : data(other.Size()) {
    std::uninitialized_copy(
      other.data.Get(), 
      (other.data.Get() + other.sz), 
      data.Get()
    );
    sz = other.Size();
  }
  Vector(Vector&& other) : data(other.Size()) {
    std::uninitialized_move(
      other.data.Get(), 
      (other.data.Get() + other.sz), 
      data.Get()
    );
    sz = other.Size();
  }

  ~Vector() {
    std::destroy_n(data.Get(), sz);
  }

  Vector& operator = (const Vector& other) {
    if(sz >= other.Size()) {
      for(size_t i = 0; i < other.sz; i++) {
        data[i] = other.data[i];
      }
      if(sz != other.Size()) {
        std::destroy_n(data.Get(), sz - other.sz);
      }
    } else {
      RawMem<T> new_mem(other.Size());
      std::uninitialized_copy(
        other.data.Get(), 
        (other.data.Get() + other.sz), 
        data.Get()
      );
      data.Swap(new_mem);
      std::destroy_n(new_mem.Get(), sz);
    }
    sz = other.Size();
    return *this;
  }

  Vector& operator = (Vector&& other) noexcept {
    if(sz >= other.Size()) {
      std::uninitialized_move(
        other.data.Get(), 
        (other.data.Get() + other.sz), 
        data.Get()
      );
      if(sz != other.Size()) {
        std::destroy_n(data.Get(), sz - other.sz);
      }
    } else {
      RawMem<T> new_mem(other.Size());
      std::uninitialized_move(
        other.data.Get(), 
        (other.data.Get() + other.sz), 
        data.Get()
      );
      data.Swap(new_mem);
      std::destroy_n(new_mem.Get(), sz);
    }
    sz = other.Size();
    return *this;
  }

  void Reserve(size_t n) {
    if(data.Cp() < n) {
      RawMem<T> new_mem(n);
      std::uninitialized_move(
        data.Get(), 
        (data.Get() + sz), 
        new_mem.Get()
      );
      data.Swap(new_mem);
      std::destroy_n(new_mem.Get(), sz);
    }
  }

  void Resize(size_t n) {
    if(data.Cp() < n) {
      Reserve(n);
    }
    std::uninitialized_default_construct_n(
      (data + sz),
      n - sz
    );
    sz = n;
  }

  void PushBack(const T& elem);
  void PushBack(T&& elem);

  template <typename ... Args>
  T& EmplaceBack(Args&&... args);

  void PopBack();

  size_t Size() const noexcept {
    return sz;
  }

  size_t Capacity() const noexcept {
    return data.Cp();
  }

  const T& operator[](size_t i) const {
    return data[i];
  }
  T& operator[](size_t i) {
    return data[i];
  }
private:
  RawMem<T> data;
  size_t sz;
};
