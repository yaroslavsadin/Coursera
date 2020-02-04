#include <cstddef>
#include <iostream>
#include <utility>
#include <memory>

template<typename T>
class RawMem {
public:
  RawMem() : data(nullptr), cp(0) {}
  RawMem(size_t n) { // may throw
    // return static_cast<T*>(operator new(n * sizeof(T)));
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
    std::swap(cp,other.cp);
    std::swap(data,other.data);
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
    std::uninitialized_default_construct_n(
        data.Get(), n
      );
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
  Vector(Vector&& other) {
    data.Swap(other.data);
    std::swap(sz,other.sz);
  }

  ~Vector() {
    std::destroy_n(data.Get(), sz);
  }

  Vector& operator = (const Vector& other) {
    if(data.Cp() >= other.Size()) {
      for(size_t i = 0; i < other.sz; i++) {
        data[i] = other.data[i];
      }
      if(sz > other.Size()) {
        std::destroy_n(data.Get(), sz - other.sz);
      }
    } else {
      RawMem<T> new_mem(other.Size());
      std::uninitialized_copy(
        other.data.Get(), 
        (other.data.Get() + other.sz), 
        new_mem.Get()
      );
      data.Swap(new_mem);
      std::destroy_n(new_mem.Get(), sz);
    }
    sz = other.Size();
    return *this;
  }

  Vector& operator = (Vector&& other) noexcept {
    data.Swap(other.data);
    std::swap(sz,other.sz);
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
    if(sz < n) {
      std::uninitialized_default_construct_n(
        (data + sz),
        n - sz
      );
    } else if(sz > n) {
      std::destroy_n(data.Get() + n, sz - n);
    }
    sz = n;
  }

  void PushBack(const T& elem) {
    if(data.Cp() == 0) {
      Reserve(1);
    }
    else if(sz == data.Cp()) {
      Reserve(sz * 2);
    }
    new (data + sz) T(elem);
    sz++;
  }
  void PushBack(T&& elem) {
    if(data.Cp() == 0) {
      Reserve(1);
    }
    else if(sz == data.Cp()) {
      Reserve(sz * 2);
    }
    new (data + sz) T(std::move(elem));
    sz++;
  }

  template <typename ... Args>
  T& EmplaceBack(Args&&... args) {
    if(data.Cp() == 0) {
      Reserve(1);
    }
    else if(sz == data.Cp()) {
      Reserve(sz * 2);
    }
    new (data + sz) T(std::forward<Args>(args)...);
    sz++;
    return data[sz-1];
  }

  void PopBack() {
    data[sz - 1].~T();
    sz--;
  }

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

  // В данной части задачи реализуйте дополнительно эти функции:
  using iterator = T*;
  using const_iterator = const T*;

  iterator begin() noexcept;
  iterator end() noexcept;

  const_iterator begin() const noexcept;
  const_iterator end() const noexcept;

  // Тут должна быть такая же реализация, как и для константных версий begin/end
  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;

  // Вставляет элемент перед pos
  // Возвращает итератор на вставленный элемент
  iterator Insert(const_iterator pos, const T& elem);
  iterator Insert(const_iterator pos, T&& elem);

  // Конструирует элемент по заданным аргументам конструктора перед pos
  // Возвращает итератор на вставленный элемент
  template <typename ... Args>
  iterator Emplace(const_iterator it, Args&&... args);

  // Удаляет элемент на позиции pos
  // Возвращает итератор на элемент, следующий за удалённым
  iterator Erase(const_iterator it);
private:
  RawMem<T> data;
  size_t sz;
};
